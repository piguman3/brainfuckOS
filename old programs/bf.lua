local args = {...}
local programfile = fs.open(args[1], "r")
local program = programfile.readAll()
programfile.close()

local memsize = 2^14
local memory = {}
for x=1,memsize do
    memory[x] = 0
end
local PC = 0
local ptr = 2^14 / 2

while 1 do
    PC = PC + 1
    if PC>#program then
        print()
        return
    end
    local inst = program:sub(PC, PC)
    if (inst==">") then 
        ptr = ptr + 1
    elseif (inst=="<") then 
        ptr = ptr - 1
    elseif (inst=="+") then 
        memory[ptr] = memory[ptr] + 1 
        if (memory[ptr]>255) then memory[ptr] = 0 end
    elseif (inst=="-") then 
        memory[ptr] = memory[ptr] - 1
        if (memory[ptr]<0) then memory[ptr] = 255 end
    elseif (inst==".") then 
        term.write(string.char(memory[ptr]))
        if memory[ptr]==10 then print() end --Line feed
    elseif (inst==",") then 
        local event, char = os.pullEvent("char")
        memory[ptr] = string.byte(char)
    elseif (inst=="[") then
        if (memory[ptr] == 0) then
            local t = 0
            for x=PC+1,#program do
                if (program:sub(x, x)=="[") then t = t + 1 end
                if (program:sub(x, x)=="]") then t = t - 1 end
                if (t<0) then PC = x; break end
            end
        end
    elseif (inst=="]") then
        if (memory[ptr] ~= 0) then
            local t = 0
            for x=PC-1,1,-1 do
                if (program:sub(x, x)=="[") then t = t - 1 end
                if (program:sub(x, x)=="]") then t = t + 1 end
                if (t<0) then PC = x; break end
            end
        end
    end
end