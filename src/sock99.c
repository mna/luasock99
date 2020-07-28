#include <errno.h>
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "socket99/socket99.h"

static char *gettablestring(lua_State *L, const char *field) {
  int typ = lua_getfield(L, -1, field);

  if (typ != LUA_TNIL && typ != LUA_TSTRING) {
    luaL_error(L, "field %s must be a string", field);
    return NULL;
  }

  char *value = NULL;
  if (typ != LUA_TNIL) {
    // must duplicate the string because we will pop it from the Lua
    // stack before using its value.
    value = strdup(lua_tostring(L, -1));
    if (value == NULL) {
      luaL_error(L, strerror(errno));
      return NULL;
    }
  }

  lua_pop(L, 1);
  return value;
}

static int gettableint(lua_State *L, const char *field) {
  int typ = lua_getfield(L, -1, field);

  int isnum;

  lua_Integer value = lua_tointegerx(L, -1, &isnum);

  if (typ != LUA_TNIL && !isnum) {
    return luaL_error(L, "field %s must be convertible to an integer", field);
  }

  lua_pop(L, 1);
  return (int)value;
}

static int gettablebool(lua_State *L, const char *field) {
  int typ = lua_getfield(L, -1, field);

  if (typ != LUA_TNIL && typ != LUA_TBOOLEAN) {
    return luaL_error(L, "field %s must be a boolean", field);
  }
  int value = lua_toboolean(L, -1);

  lua_pop(L, 1);
  return value;
}

static int sock99_open(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);

  socket99_config cfg = { 0 };
  socket99_result res;

  cfg.host         = gettablestring(L, "host");
  cfg.port         = gettableint(L, "port");
  cfg.path         = gettablestring(L, "path");
  cfg.IPv4         = gettablestring(L, "ipv4");
  cfg.IPv6         = gettablestring(L, "ipv6");
  cfg.server       = gettablebool(L, "server");
  cfg.datagram     = gettablebool(L, "datagram");
  cfg.nonblocking  = gettablebool(L, "nonblocking");
  cfg.backlog_size = gettableint(L, "backlog");
  // TODO: array of options

  /*
   * printf("\nhost:%s;port:%d;path:%s;ip4:%s;ip6:%s;server:%d;datagram:%d;nonblock:%d;backlog:%d\n",
   *     cfg.host,
   *     cfg.port,
   *     cfg.path,
   *     cfg.IPv4,
   *     cfg.IPv6,
   *     cfg.server,
   *     cfg.datagram,
   *     cfg.nonblocking,
   *     cfg.backlog_size);
   */

  bool ok = socket99_open(&cfg, &res);
  if (!ok && res.fd > 0) {
    close(res.fd);
  }

  if (cfg.host) {
    free(cfg.host);
  }
  if (cfg.path) {
    free(cfg.path);
  }
  if (cfg.IPv4) {
    free(cfg.IPv4);
  }
  if (cfg.IPv6) {
    free(cfg.IPv6);
  }

  if (ok) {
    lua_pushinteger(L, (lua_Integer)res.fd);
    return 1;
  } else {
    lua_pushnil(L);
    if (res.status == SOCKET99_ERROR_GETADDRINFO) {
      lua_pushstring(L, gai_strerror(res.getaddrinfo_error));
      lua_pushinteger(L, res.getaddrinfo_error);
    } else {
      lua_pushstring(L, strerror(res.saved_errno));
      lua_pushinteger(L, res.saved_errno);
    }
    return 3;
  }
}

static const luaL_Reg mod[] = {
  { "open", sock99_open },
  { NULL,   NULL        }
};

int luaopen_sock99(lua_State *L) {
  luaL_newlib(L, mod);
  return 1;
}
