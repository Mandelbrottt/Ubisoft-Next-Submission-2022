local function mysplit (inputstr, sep)
    if sep == nil then
       sep = "%s"
    end
    local t={}
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
       table.insert(t, str)
    end
    return t
end

-- https://stackoverflow.com/a/23535333
function script_dir()
    local str = debug.getinfo(2, "S").source:sub(2)
    local path = str:match("(.*[/\\])") or "."
    local split = mysplit(path, "/\\")
    local last_dir
    for _, v in ipairs(split) do
        last_dir = v
    end
    return last_dir
end

function replace_string_in_file(filename, find_string, replace_string)
    
end