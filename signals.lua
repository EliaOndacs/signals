Event = {}
Event.__index = Event

function Event.new(object, attributes, id)
    local self = setmetatable({}, Event)
    self.object = object
    self.id = id
    self.attributes = attributes
    return self
end

Clot = {}
Clot.__index = Clot

function Clot.new(event, sender, callback)
    local self = setmetatable({}, Clot)
    self.event = event
    self.sender = sender
    self.callback = callback
    return self
end

function Clot:call(...)
    self.callback(self.event, ...)
end

Signal = {}
Signal.__index = Signal

function Signal.new()
    local self = setmetatable({}, Signal)
    self.subscriptions = {}
    return self
end

function Signal:subscribe(obj)
    table.insert(self.subscriptions, Clot.new(obj, obj, obj.__clot__))
end

function Signal:unsubscribe(obj)
    for i, clot in ipairs(self.subscriptions) do
        if clot.sender == obj then
            table.remove(self.subscriptions, i)
            break
        end
    end
end

function Signal:publish(data)
    for _, clot in ipairs(self.subscriptions) do
        clot:call(data)
    end
end

function Signal:getSubscriptions()
    return self.subscriptions
end
