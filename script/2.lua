local meta = {}
local vector3d = {}

function vector3d.new(x, y, z)
    local obj = {x = x, y = y, z = z}
    setmetatable(obj, meta)
    return obj
end

function vector3d.add(v1, v2)
    return vector3d.new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z)
end


function vector3d.toString(v)
    return string.format("Vector3D(%f, %f, %f)", v.x, v.y, v.z)
end

position = vector3d.new(1, 2, 3)
velocity = vector3d.new(4, 5, 6)
local result = vector3d.add(position, velocity)
print(vector3d.toString(result)) -- 输出: Vector3D(5.000000, 7.000000, 9.000000)