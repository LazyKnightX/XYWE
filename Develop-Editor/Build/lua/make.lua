package.path = package.path .. ';' .. arg[1] .. '/luabuild/src/?.lua' .. ';' .. arg[1] .. '/?.lua'
package.cpath = package.cpath .. ';' .. arg[1] .. '/luabuild/bin/?.dll'

local configuration = arg[2] or 'Debug'
local dev = arg[4] ~= nil and arg[4] == 'Dev'
local update = arg[4] ~= nil and arg[4] == 'Update'

local oldprint = print
if type(arg[3]) == 'string' then
	local out = io.open(arg[3], 'w')
	if out then
		function print(...)
			out:write(table.concat(table.pack(...), '\t') .. '\n')
			oldprint(...)
		end
	end
end

require 'luabind'
require 'filesystem'
local zip = require 'zip'
local uni = require 'unicode'
local filelock = require 'filelock'
local buildroot = fs.path(uni.a2u(arg[1])) / '..'

local oklock = filelock(buildroot / 'ok.lock')

oklock:unlock()

-- Step.1 初始化
local msvc = require 'msvc'
if not msvc:initialize(120) then
	error('Cannot found Visual Studio Toolset.')
end

local root = require 'path'
if not root then
	error('Not found `build.root`.')
end

local path = {}
path.Root = fs.path(root)
path.Build = path.Root / 'Build'
path.ThirdParty = path.Root / 'ThirdParty'
path.OpenSource = path.Root / 'OpenSource'
path.Development = path.Root / 'Development'
path.Result = path.Root / '..' / 'core' / 'editor'

-- Step.2 清理
if not update then
	fs.remove_all(path.Development / 'Build' / 'bin' / configuration)
	fs.remove_all(path.Development / 'Build' / 'obj' / configuration)
	fs.remove_all(path.Development / 'Build' / 'lib' / configuration)
	fs.remove_all(path.Build / 'publish' / configuration)
end

-- Step.3 版本信息
local function split(str, p)
	local rt = {}
	str:gsub('[^'..p..']+', function (w) table.insert(rt, w) end)
	return rt
end
if fs.exists(path.Build / 'include'/ 'version') then
	fs.create_directories(path.Development / 'Build' / 'include')
	local f = assert(io.open(uni.u2a((path.Build / 'include'/ 'version'):string()), 'r'))
	local version = f:read 'a'
	f:close()
	local major, minor, revised, build = table.unpack(split(version, '.'))
	local major, minor, revised, build = tonumber(major), tonumber(minor), tonumber(revised), tonumber(build)
	local f = assert(io.open(uni.u2a((path.Development / 'Build' / 'include' / 'YDWEVersion.h'):string()), 'w'))
	f:write(([[
#ifndef YDWE_VERSION_H_INCLUDED
#define YDWE_VERSION_H_INCLUDED

#define YDWE_VERSION_MAJOR %d
#define YDWE_VERSION_MINOR %d
#define YDWE_VERSION_REVISED %d
#define YDWE_VERSION_BUILD %d

#endif // YDWE_VERSION_H_INCLUDED
	]]):format(major, minor, revised, build))
	f:close()
	print(('build %d.%d.%d.%d'):format(major, minor, revised, build))
end

-- Step.4 编译
if not update then
	msvc:rebuild(path.ThirdParty / 'Microsoft' / 'Detours' / 'Current' / 'sln' / 'Detours.sln', configuration)
	--msvc:rebuild(path.OpenSource / 'StormLib' / 'Current' / 'StormLib.sln', configuration)
	--msvc:rebuild(path.OpenSource / 'sfmpq' / 'project' / 'sfmpq.sln', configuration)
	msvc:rebuild(path.OpenSource / 'minizip' / 'project' / 'minizip.sln', configuration)
	msvc:rebuild(path.OpenSource / 'all.sln', 'Release')
	msvc:rebuild(path.OpenSource / 'Lua' / 'Current' / 'makefiles' / 'luacore.sln', configuration)
	msvc:rebuild(path.OpenSource / 'luaffi' / 'makefiles' / 'luaffi.sln', configuration)
	msvc:rebuild(path.Development / 'Plugin' / 'YDColorizer' / 'YDColorizer.sln', configuration, 'Any CPU')
	if not dev then
		msvc:rebuild(path.Development / 'Core' / 'Solution' / 'YDWE.sln', configuration)
	end
end

-- Step.5 复制
local function get_boost_version()
	local f = io.open((path.OpenSource / 'Boost' / 'boost' / 'version.hpp'):string(), 'rb')
	if not f then
		error('Not found <boost/version.hpp>')
	end
	local content = f:read 'a'
	f:close()
	local version = content:match('BOOST_LIB_VERSION%s+"([%d_]+)"')
	if not version then
		error('Error in <boost/version.hpp>')
	end
	return version
end

local function copy_boost_dll(name)
	local fmt
	if configuration == 'Release' then
		fmt = 'boost_%s-vc%s-mt-%s.dll'
	else
		fmt = 'boost_%s-vc%s-mt-gd-%s.dll'
	end
	local filename = fmt:format(name, msvc.version, get_boost_version())
	fs.copy_file(path.OpenSource / 'Boost' / 'stage' / 'lib' / filename, path.Result / 'bin' / filename, true)
end

local function copy_directory(from, to, filter)
	fs.create_directories(to)
	for fromfile in from:list_directory() do
		if fs.is_directory(fromfile) then
			copy_directory(fromfile, to / fromfile:filename(), filter)
		else
			if (not filter) or filter(fromfile) then
				fs.copy_file(fromfile, to / fromfile:filename(), true)
			end
		end
	end
end

local function copy_crt_dll()
	if configuration ~= 'Release' then
		return
	end
	local crt_path = fs.path(msvc.path) / 'redist' / 'x86' / ('Microsoft.VC' .. msvc.version .. '.CRT')
	if fs.exists(crt_path) then
		copy_directory(crt_path, path.Result / 'bin', function(path)
			local ext = path:extension():string():lower()
			return ext == '.dll'
		end)
	end
end

fs.create_directories(path.Result / 'bin' / 'modules')
fs.create_directories(path.Result / 'plugin' / 'jasshelper' / 'bin')
copy_crt_dll() -- msvcp120.dll, msvcr120.dll
copy_boost_dll('system')
copy_boost_dll('filesystem')
fs.copy_file(path.OpenSource / 'Lua' / 'current' / 'bin' / 'Win32' / configuration / 'luacore.dll', path.Result / 'bin' / 'luacore.dll', true)
fs.copy_file(path.OpenSource / 'Lua' / 'current' / 'bin' / 'Win32' / configuration / 'luabind.dll', path.Result / 'bin' / 'luabind.dll', true)
--fs.copy_file(path.OpenSource / 'StormLib' / 'current' / 'bin' / 'Win32' / configuration / 'StormLib.dll', path.Result / 'bin' / 'StormLib.dll', true)
fs.copy_file(path.OpenSource / 'minizip' / 'bin' / configuration / 'minizip.dll', path.Result / 'bin' / 'minizip.dll', true)
fs.copy_file(path.OpenSource / 'luaffi' / 'bin' / configuration / 'ffi.dll', path.Result / 'bin' / 'modules' / 'ffi.dll', true)
--fs.copy_file(path.OpenSource / 'sfmpq' / 'bin' / configuration / 'sfmpq.dll', path.Result / 'plugin' / 'jasshelper' / 'sfmpq.dll', true)
--fs.copy_file(path.OpenSource / 'sfmpq' / 'bin' / configuration / 'sfmpq.dll', path.Result / 'plugin' / 'jasshelper' / 'bin' / 'sfmpq.dll', true)
copy_directory(path.OpenSource / 'Boost' / 'boost' / 'preprocessor', path.Result / 'plugin' / 'wave' / 'include' / 'boost' / 'preprocessor')
fs.copy_file(path.OpenSource / 'Boost' / 'boost' / 'preprocessor.hpp', path.Result / 'plugin' / 'wave' / 'include' / 'boost' / 'preprocessor.hpp', true)
copy_directory(path.Development / 'Component', path.Result)

-- Step.6 打包皮肤
zip(path.Development / 'Core' / 'DuiLib' / 'Resources', path.Result / 'bin' / 'skin.zip', 
	function (file) 
		return file:string():lower() ~= 'thumbs.db'
	end
)

oklock:lock()

local time = math.floor(os.clock())
print(('time: %d:%02d'):format(time//60, time%60))
