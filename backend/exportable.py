import json
from typing import Any, Dict, Optional, List

def remove_private(value: str) -> str:
    if '__' not in value:
        return value
    return value.split('__', 1)[1]

class Exportable:
    def __init__(self, ignore: Optional[List[str]] = None) -> None:
        self.__blacklist = ['blacklist']
        if ignore is not None:
            self.__blacklist += ignore

    def to_json(self) -> str:
        return Exportable.export_object(obj=self, blacklist=self.__blacklist)

    def to_dict(self) -> Dict[str, Any]:
        return Exportable.export_to_dict(obj=self, blacklist=self.__blacklist)

    def __repr__(self) -> str:
        return self.to_json()

    @staticmethod
    def export_object(obj: Any, blacklist: Optional[List[str]] = None) -> str:
        check_blacklist: List[str] = []
        if blacklist is not None:
            check_blacklist = blacklist
        return json.dumps(obj,
                          default=lambda o: {remove_private(a): b for a, b in o.__dict__.items()
                                             if remove_private(a) not in check_blacklist},
                          sort_keys=True)

    @staticmethod
    def export_to_dict(obj: Any, blacklist: Optional[List[str]] = None) -> Dict[str, Any]:
        if blacklist is None:
            blacklist = []
        data: Dict[str, Any] = json.loads(Exportable.export_object(obj=obj, blacklist=blacklist))
        return data
