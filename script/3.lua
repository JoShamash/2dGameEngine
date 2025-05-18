Character = {
    HP = 0,
    MP = 0,
    ATK = 0,
    DEF = 0,
    EXP = 0,
    LVL = 0,
}

-- 角色类，属性有：血量，魔法值，攻击力，防御力，经验值，等级

-- 角色类的构造函数
function Character:new(o, hp, mp, atk, def, exp, lvl)
    o = o or {}  -- 如果没有传入 o，就创建一个新的空表
    setmetatable(o, self)
    self.__index = self  -- 设置元表

    -- 给实例 o 赋值属性
    o.HP  = o.HP  or hp or 0
    o.MP  = o.MP  or mp or 0
    o.ATK = o.ATK or atk or 0
    o.DEF = o.DEF or def or 0
    o.EXP = o.EXP or exp or 0
    o.LVL = o.LVL or lvl or 0

    return o
end

-- 角色类的升级函数
function Character:levelUp()
    self.LVL = self.LVL + 1
    self.HP = self.HP + 10
    self.MP = self.MP + 5
    self.ATK = self.ATK + 2
    self.DEF = self.DEF + 2
    print("Level Up! New Level: " .. self.LVL)
end

-- 角色类的信息函数
function Character:info()
    print("Character Info:")
    print("HP: " .. self.HP)
    print("MP: " .. self.MP)
    print("ATK: " .. self.ATK)
    print("DEF: " .. self.DEF) 
    print("EXP: " .. self.EXP)
    print("LVL: " .. self.LVL)
end

-- 正确的参数形式：传入一个带有字段名的表
local player = Character:new({HP = 100, MP = 50, ATK = 20, DEF = 10, EXP = 0, LVL = 1})
player:info()
player:levelUp()
player:info()
