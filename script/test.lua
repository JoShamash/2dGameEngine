local students = {}

local function addStudent(id, name, major)
    students[id] = { name = name, major = major }
end

local function removeStudent(id)
    students[id] = nil
end

local function getStudent(id)
    return students[id]
end

local function listStudents()
    for id, info in pairs(students) do
        print("ID: "..id, "Name: "..info.name, "Major: "..info.major)
    end
end

-- Example usage:
addStudent("001", "Alice我", "Computer Science")
addStudent("002", "Bob", "Math")
listStudents()
removeStudent("001")
listStudents()