from typing import Any, Callable
from typing import TypeVar

HasClot = TypeVar("HasClot")
"""any object that has a `__clot__` method that returns a `Clot` object"""


class Event:
    def __init__(
        self, object: Any, attributes: dict[str, Any], id: str | None = None
    ) -> None:
        self.__dict__.update(attributes)
        self.object = object
        self.id = id


class Clot:
    def __init__(self, event: Event, sender: Any, callback: Callable) -> None:
        self.callback = callback
        self.event = event
        self.sender = sender

    def __call__(self, *args: Any, **kwds: Any) -> Any:
        return self.callback(self.event, *args, **kwds)

class Signal[SignalT]:
    __slots__ = ["_subscriptions"]

    def __init__(self) -> None:
        self._subscriptions: list[Clot] = []

    def subscribe(self, obj: HasClot):
        self._subscriptions.append(obj.__clot__())

    def unsubscribe(self, obj: HasClot):
        for clot in self.subscriptions:
            if clot.sender == obj:
                self._subscriptions.remove(clot)

    def publish(self, data: SignalT):
        for clot in self.subscriptions:
            clot(data)

    @property
    def subscriptions(self):
        return self._subscriptions


