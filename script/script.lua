-- 测试代码示例：简单的Lua脚本
-- 定义一个函数
function greet(name)
    return "Hello, " .. name .. "!"
end

-- 调用函数并打印结果
local message = greet("World")
print(message)

-- 定义一个表（类似于字典或对象）
local player = {
    name = "Player1",
    score = 100
}

-- 更新表中的值
player.score = player.score + 50

-- 打印表中的值
print("Name: " .. player.name)
print("Score: " .. player.score)

-- 循环示例
for i = 1, 5 do
    print("Count: " .. i)
end