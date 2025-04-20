"""
[
    1,
    2,
    3,
    [
        "a",
        "b",
        [
            "c",
        ],
        "foo",
    ],
    4,
]
"""
def my_pprint(itemOrList):
    result = []

    def inner(itemOrList, indent, result):
        comma = "" if len(indent) == 0 else ","

        if isinstance(itemOrList, list):
            result.append(f"\n{indent}[")
            for item in itemOrList:
                inner(item, indent + "  ", result)
            result.append(f"\n{indent}]{comma}")
        else:
            result.append(f"\n{indent}{repr(itemOrList)}{comma}")

    inner(itemOrList, "", result)

    print("".join(result))

if __name__ == "__main__":
    l = [1, 2, 3, ["a", "b", ["c"], "foo"], 4]
    my_pprint(l)
