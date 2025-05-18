math.randomseed(os.time())
local time = os.date("%Y-%m-%d %H:%M:%S")

local isRunning = true
local player = {x = 50, y = 50}
local enemy = {x = 0, y = 0}

local function PrintMenu()
    print("+------------------------------------------------+")
    print("|         Welcome, "..time.."           |")
    print("+------------------------------------------------+")
    print("|        1.Generate random enemy position        |")
    print("|        2.Distance from enemy to player         |")
    print("|        3.Get angle from enemy to player        |")
    print("|        4.Exit                                  |")
end

local function Tool_1()
    enemy.x = math.random(1, 100)
    enemy.y = math.random(1, 100)
end

local function Tool_2()
    return math.sqrt((enemy.x - player.x)^2 + (enemy.y - player.y)^2)
end

local function Tool_3()
    return math.deg(math.atan(player.y - enemy.y, player.x - enemy.x))
end

local function Tool_4()

end 

while isRunning do
    PrintMenu()
    local choice = io.read("n")
    if choice == 1 then
        Tool_1()
        print("Enemy position: ("..enemy.x..", "..enemy.y..")")
    elseif choice == 2 then
        local distance = Tool_2()
        print("Distance from enemy to player: " .. distance)
    elseif choice == 3 then
        local angle = Tool_3()
        print("Angle from enemy to player: " .. angle)
    elseif choice == 4 then
        isRunning = false
        
        print("Exiting...")
    else
        print("Invalid choice. Please try again.")
    end
end
