local global_args = {...}
local programfile = fs.open(global_args[1], "r")
if not programfile then
    print("Error opening file "..global_args[1])
    return
end
local program = programfile.readAll()
programfile.close()

local root = "/bfroot/"
if not fs.exists(root) then
    fs.makeDir(root)
end

local memsize = 2^14
local memory = {}
for x=1,memsize do
    memory[x] = 0
end
local PC = 0
local ptr = 2^14 / 2
local syscallsize = 4

local exeargs = {}
if #global_args>1 then
    for x=2, #global_args do
        exeargs[x-1] = global_args[x]
    end
end

local function strtotable(input)
    local t = {}
    for x=1,#input do
        t[x] = string.byte(input:sub(x, x))
    end
    t[#input + 1] = 0
    return t
end

local function chartoint(str)
    local res = 0
    for x=1,#str do
        res = res + str[x] * math.pow(256, (#str-x))
    end
    return res
end

local function inttochar(int, size)
    local t = {}
    local nsize = math.ceil(math.log(int)/math.log(255))
    for x=1,size do
        if size-x>=nsize then
            t[x] = 0
        else
            t[x] = math.floor((int / (math.pow(256, size-x)) % 256 + 0.5))
        end
    end
    return t
end

local function tabletostr(t)
    return table.concat({string.char(table.unpack(t))}):sub(1,-2) --Remove \0
end

local fileslots = {}
local modemap = {
    ["rt"] = "r",
    ["wt"] = "w",
    ["at"] = "a",
    ["rb"] = "rb",
    ["wb"] = "wb",
    ["ab"] = "ab",
}

local syscalls = { --4 chars for each syscall
    --I/O
    ["PRNT"] = function(args) --Print text
        write(tabletostr(args))
    end,
    ["READ"] = function(args) --read()
        local input = read()
        return strtotable(input)
    end,
    ["SLEP"] = function(args) --Sleep
        local ax = chartoint(args)
        sleep(args[1]*0.05)
    end,
    ["SCPX"] = function(args) --Get/set cursor X
        local x, y = term.getCursorPos()
        local ax = chartoint(args)
        term.setCursorPos(ax+1, y)
    end,
    ["SCPY"] = function(args) --Get/set cursor Y
        local x, y = term.getCursorPos()
        local ay = chartoint(args)
        term.setCursorPos(x, ay+1)
    end,
    ["GCPX"] = function(args) --Get/set cursor X
        local x, y = term.getCursorPos()
        return inttochar(x, 2)
    end,
    ["GCPY"] = function(args) --Get/set cursor Y
        local x, y = term.getCursorPos()
        return inttochar(y, 2)
    end,
    ["CLRS"] = function(args) --Clear screen
        term.clear()
    end,
    ["GETW"] = function(args) --Get width
        return inttochar(select(1, term.getSize()), 2)
    end,
    ["GETH"] = function(args) --Get height
        return inttochar(select(2, term.getSize()), 2)
    end,
    ["SCRL"] = function(args) --Scroll
        term.scroll(chartoint(args))
    end,
    ["CHAR"] = function(args) --Pull single char event
        return {string.byte(select(2, os.pullEvent("char")))}
    end,
    --Filesystem, absolute paths
    ["OPEN"] = function(args) --Open
        local sargs = tabletostr(args)
        if (args[1] == 0x25) and (args[2] == 0x4F) then
            term.write("9551\n")
            return {0}
        end
        local mode = sargs:sub(1, 2)
        local filepath = root .. sargs:sub(3, -1)

        for x=1,255 do
            if fileslots[x]==nil then --Fill open slot
                local file, err = fs.open(filepath, assert(modemap[mode], ("Mode doesn't exist: " .. tostring(mode))))
                if file==nil then
                    return {0} -- error
                else
                    fileslots[x] = file
                    return {x}
                end
            end
        end
        error("No more file slots left")
    end,
    ["CLOS"] = function(args) --Close
        local fileslot = args[1]
        if fileslots[fileslot]~=nil then
            fileslots[fileslot].close()
            fileslots[fileslot] = nil
        end
    end,
    ["REAB"] = function(args) --Read bytes
        local fileslot = args[1]
        local size = chartoint({args[2], args[3]})
        if (fileslots[fileslot]~=nil) and (fileslots[fileslot].read) then
            local readstr = fileslots[fileslot].read(size)
            if readstr==nil then 
                return {0}
            end
            return strtotable(readstr)
        end
    end,
    ["WRTE"] = function(args) --Write bytes
        local fileslot = args[1]
        local buf = tabletostr(args):sub(2, -1)
        if (fileslots[fileslot]~=nil) and (fileslots[fileslot].write) then
            fileslots[fileslot].write(buf)
        end
    end,
    ["SIZE"] = function(args) --Get size of file
        if fs.exists(root .. tabletostr(args)) then
            return inttochar(fs.getSize(root .. tabletostr(args)), 2)
        else
            return {0}
        end
    end,
    ["LSSZ"] = function(args) --List files in directory
        if fs.exists(root .. tabletostr(args)) then
            return {#fs.list(root .. tabletostr(args))%256}
        else
            return {0}
        end
    end,
    ["GTLS"] = function(args) --Get file in index of directory
        local sarg = tabletostr(args)
        local fileindex = args[1] or 1
        local dirpath = root .. sarg:sub(2, -1)
        local file = fs.list(dirpath)[fileindex]
        if file then
            local output 
            if fs.isDir(dirpath .. file) then --Add a char for if it's a file or a directory
                output = "D"
            else
                output = "F"
            end
            output = output .. file
            return strtotable(output)
        end
    end,
    ["EXEC"] = function(args) --Execute file with interpreter
        local ok, err = pcall(
            function()
                loadfile("bfk.lua")(root .. tabletostr(args), table.unpack(exeargs))
            end
        )
        if not ok then
            print(err)
        end
    end,
    ["MDIR"] = function(args) --Make directory
        fs.makeDir(root .. tabletostr(args))
    end,
    ["DELF"] = function(args) --Delete path
        fs.delete(root .. tabletostr(args))
    end,
    ["FCPY"] = function(args) --Make directory
        local fstringsize = args[1]
        local string = tabletostr(args):sub(2, -1)
        local filea = string:sub(1, fstringsize)
        local fileb = string:sub(fstringsize + 1, -1)
        fs.copy(root .. filea, root .. fileb)
    end,
    ["FMOV"] = function(args) --Move file
        local fstringsize = args[1]
        local string = tabletostr(args):sub(2, -1)
        local filea = string:sub(1, fstringsize)
        local fileb = string:sub(fstringsize + 1, -1)
        fs.move(root .. filea, root .. fileb)
    end,
    ["EDIR"] = function(args) --Get if directory exists
        local file = root .. tabletostr(args)
        if fs.exists(file) then
            return {fs.isDir(file) and 1 or 0} --Return 1 or 0
        else
            return {0}
        end
    end,
    ["EFIL"] = function(args) --Get if file exists
        local file = root .. tabletostr(args)
        if fs.exists(file) then
            return {(not fs.isDir(file)) and 1 or 0} --Return 1 or 0
        else
            return {0}
        end
    end,
    ["GARG"] = function(args) --Get arguments
        local index = args[1]
        return strtotable(exeargs[index] or "")
    end,
    ["SARG"] = function(args) --Set arguments
        local arg = tabletostr(args):sub(2, -1)
        local index = args[1]
        exeargs[index] = arg
    end
}

local syscalltype = { --Maps if syscall should perform with a null char or a size (false = null, 0 = no params, >0 param char count)
    ["PRNT"] = false,
    ["READ"] = 0,
    ["SLEP"] = 2,
    ["SCPX"] = 2,
    ["SCPY"] = 2,
    ["CLRS"] = 0,
    ["GETW"] = 0,
    ["GETH"] = 0,
    ["SCRL"] = 2,
    ["CHAR"] = 0,
    ["GCPX"] = 0,
    ["GCPY"] = 0,
    ["OPEN"] = false,
    ["CLOS"] = 1,
    ["REAB"] = 3,
    ["SIZE"] = false,
    ["LSSZ"] = false,
    ["GTLS"] = false,
    ["EXEC"] = false, --TODO: implement this
    ["MDIR"] = false,
    ["DELF"] = false,
    ["WRTE"] = false,
    ["EDIR"] = false,
    ["EFIL"] = false,
    ["GARG"] = 1,
    ["SARG"] = false,
    ["FCPY"] = false,
    ["FMOV"] = false
}

local currentreturn = {}
local currentcall = {}
local calltype = -1
local syscallname = ""

local function dosyscall(syscallname)
    local call = assert(syscalls[syscallname], "Syscall doesn't exist")
    for x=1,syscallsize do
        table.remove(currentcall, 1)
    end
    currentreturn = call(currentcall) or {0}
    calltype = -1
    currentcall = {}
end

local supported = "><+-.,"
local code = {}
local metacode = {}
local cptr = 0
while cptr<#program+1 do
    cptr = cptr + 1
    local char = program:sub(cptr,cptr)
    code[cptr] = string.byte(char)
    local sptr = cptr
    if code[cptr] == 0x5B then --[
        local t = 0
        for x=cptr+1,#program do
            if (program:sub(x,x)=="[") then t = t + 1 end --[
            if (program:sub(x,x)=="]") then t = t - 1 end --]
            if (t<0) then sptr = x; break end
        end
        metacode[cptr] = sptr
    elseif code[cptr] == 0x5D then --]
        local t = 0
        for x=cptr-1,1,-1 do
            if (program:sub(x,x)=="[") then t = t - 1 end --[
            if (program:sub(x,x)=="]") then t = t + 1 end --]
            if (t<0) then sptr = x; break end
        end
        metacode[cptr] = sptr
    else
        for y=1,#supported do
            if char == supported:sub(y,y) then
                code[cptr] = string.byte(char)
                local sptr = cptr
                while (program:sub(sptr,sptr)==char) do 
                    sptr = sptr + 1 
                    code[sptr] = string.byte(char)
                end
                metacode[cptr] = sptr-cptr
                cptr = sptr-1
                break
            end
        end
    end
end

local instructions = {
    [">"] = function()
        ptr = ptr + metacode[PC]
        PC = PC + metacode[PC]-1
    end,
    ["<"] = function() 
        ptr = ptr - metacode[PC]
        PC = PC + metacode[PC]-1
    end,
    ["+"] = function()
        memory[ptr] = memory[ptr] + metacode[PC]%256
        PC = PC + metacode[PC]-1
        if (memory[ptr]>255) then memory[ptr] = memory[ptr]%256 end
    end,
    ["-"] = function()
        memory[ptr] = memory[ptr] - metacode[PC]%256
        PC = PC + metacode[PC]-1
        if (memory[ptr]<0) then memory[ptr] = memory[ptr]%256 end
    end,
    ["."] = function() --MAKE A SYSCALL
        -- Debug 
        --print(string.char(memory[ptr]))
        table.insert(currentcall, memory[ptr])
        if (#currentcall>=syscallsize) then
            if (calltype==-1) then
                syscallname = ""
                for x=1,syscallsize do
                    syscallname = syscallname .. string.char(currentcall[x])
                end
                calltype = syscalltype[syscallname]
                if calltype==nil then 
                    error("Syscall doesn't exist")
                end
            elseif (calltype==false) then --Null based syscall
                if (memory[ptr]==0) then --Reached null char, do syscall
                    dosyscall(syscallname)
                end
            end
            --Size based syscall
            if (calltype~=false) and (#currentcall>=calltype+syscallsize) then
                dosyscall(syscallname)
            end
        end
    end,
    [","] = function() --GET RETURN OF SYSCALL
        memory[ptr] = currentreturn[1] or 0
        table.remove(currentreturn, 1) --Pop character from the return value
    end,
    ["["] = function()
        if (memory[ptr] == 0) then
            PC = metacode[PC]
        end
    end,
    ["]"] = function()
        if (memory[ptr] ~= 0) then
            PC = metacode[PC]
        end
    end
}

local width = term.getSize()
local loading = {"\\", "|", "/", "-"}

local starttime = os.epoch("utc")
local cycle = 0
while 1 do
    PC = PC + 1
    cycle = cycle + 1
    if PC>#program then
        --print("Time taken: " .. tostring(os.epoch("utc") - starttime))
        return
    end
    local inst = string.char(code[PC])
    --print(PC)
    --read()
    if (cycle%200000)==0 then
        local oldx, oldy = term.getCursorPos()
        term.setCursorPos(width, 1)
        term.write(loading[(cycle/200000)%4+1])
        term.setCursorPos(oldx, oldy)
    end
    if instructions[inst] then
        instructions[inst]()
    end
end