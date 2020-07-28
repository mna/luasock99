local lu = require 'luaunit'
local sock99 = require 'sock99'
local socket = require 'posix.sys.socket'
local unistd = require 'posix.unistd'

TestInvalid = {}
function TestInvalid.test_nil()
  lu.assertErrorMsgContains('table expected', function()
    sock99.open()
  end)
end

function TestInvalid.test_string()
  lu.assertErrorMsgContains('table expected', function()
    sock99.open('abc')
  end)
end

TestPath = {}
function TestPath.test_server()
  local fd, err, code = sock99.open{
    path = 'unix.out',
    server = true
  }
  lu.assertNotNil(fd, err)
  lu.assertTrue(fd > 0)
  lu.assertNil(err)
  lu.assertNil(code)
  assert(unistd.close(fd))
  os.remove('unix.out')
end

TestTCP = {}
function TestTCP.test_server()
  local fd, err, code = sock99.open{
    host = '127.0.0.1',
    port = 8081,
    server = true,
  }
  lu.assertNotNil(fd, err)
  lu.assertTrue(fd > 0)
  lu.assertNil(err)
  lu.assertNil(code)
  assert(unistd.close(fd))
end

TestUDP = {}
function TestUDP.test_open()
  local fd, err, code = sock99.open{
    host = '127.0.0.1',
    port = 8082,
    datagram = true,
  }
  lu.assertNotNil(fd, err)
  lu.assertTrue(fd > 0)
  lu.assertNil(err)
  lu.assertNil(code)
  assert(unistd.close(fd))
end

function TestUDP.test_send()
  local fd = sock99.open{
    host = '127.0.0.1',
    port = 8083,
    datagram = true,
    nonblocking = true,
  }
  lu.assertNotNil(fd)

  local n = assert(socket.sendto(fd, 'allo', {
    family = socket.AF_INET,
    port = 8083,
    addr = '127.0.0.1',
  }))
  lu.assertEquals(n, 4)

  assert(unistd.close(fd))
end

os.exit(lu.LuaUnit.run(), true)
