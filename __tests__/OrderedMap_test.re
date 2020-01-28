module M = Map.OrderedMap;
open Jest;
open Expect;

module D = Js.Dict;
module S = Js.String;
module A = Belt.Array;


describe("ordering", () => {
  let map =
    [|("A", "Candy Bars"), ("B", "Peanut Butter Cups"), ("C", "Ice Cream")|] |> M.fromArray;
  let empty = M.empty();

  test("first", () => expect(M.first(map)) |> toEqual(Some("Candy Bars")));
  test("firstKey", () => expect(M.firstKey(map)) |> toEqual(Some("A")));
  test("firstPair", () => expect(M.firstPair(map)) |> toEqual(Some(("A", "Candy Bars"))));
  test("last", () => expect(M.last(map)) |> toEqual(Some("Ice Cream")));
  test("lastKey", () => expect(M.lastKey(map)) |> toEqual(Some("C")));
  test("lastPair", () => expect(M.lastPair(map)) |> toEqual(Some(("C", "Ice Cream"))));

  test("first when empty", () => expect(M.first(empty)) |> toEqual(None));
  test("firstKey when empty", () => expect(M.firstKey(empty)) |> toEqual(None));
  test("firstPair when empty", () => expect(M.firstPair(empty)) |> toEqual(None));
  test("last when empty", () => expect(M.last(empty)) |> toEqual(None));
  test("lastKey when empty", () => expect(M.lastKey(empty)) |> toEqual(None));
  test("lastPair when empty", () => expect(M.lastPair(empty)) |> toEqual(None));

  describe("exception-throwing", () => {
  test("firstExn", () => expect(M.firstExn(map)) |> toEqual(("Candy Bars")));
  test("firstKeyExn", () => expect(M.firstKeyExn(map)) |> toEqual(("A")));
  test("firstPairExn", () => expect(M.firstPairExn(map)) |> toEqual((("A", "Candy Bars"))));
  test("lastExn", () => expect(M.lastExn(map)) |> toEqual(("Ice Cream")));
  test("lastKeyExn", () => expect(M.lastKeyExn(map)) |> toEqual(("C")));
  test("lastPairExn", () => expect(M.lastPairExn(map)) |> toEqual((("C", "Ice Cream"))));

})
});

describe("mapping", () => {
  let map =
    [|("A", "Candy Bars"), ("B", "Peanut Butter Cups"), ("C", "Ice Cream")|] |> M.fromArray;

  describe("map (data structure first argument)", () => {
    test("map values only", () =>
      expect(M.map(map, S.length) |> M.valueArray) |> toEqual([|10, 18, 9|])
    );
    test("map with key", () =>
      expect(M.mapWithKey(map, (k, s) => k ++ s) |> M.valueArray)
      |> toEqual([|"ACandy Bars", "BPeanut Butter Cups", "CIce Cream"|])
    );
  });
  describe("each (data structure last argument)", () => {
    test("each values only", () =>
      expect(map |> M.each(S.length) |> M.valueArray) |> toEqual([|10, 18, 9|])
    );
    test("each with key", () =>
      expect(map |> M.eachWithKey((k, s) => k ++ s) |> M.valueArray)
      |> toEqual([|"ACandy Bars", "BPeanut Butter Cups", "CIce Cream"|])
    );
  });
});

describe("construction", () => {
  test("singleton", () =>
    expect(M.singleton("a", 1) |> M.toObject) |> toEqual([%raw {| {a: 1} |}])
  );
  test("pair", () =>
    expect(M.pair("a", 1, "b", 2) |> M.toObject) |> toEqual([%raw {| {a: 1, b: 2} |}])
  );
  test("triple", () =>
    expect(M.triple("a", 1, "b", 2, "c", 3) |> M.toObject)
    |> toEqual([%raw {| {a: 1, b: 2, c: 3} |}])
  );
  test("fromList", () =>
    expect(M.fromList([("a", 1), ("b", 2)]) |> M.toObject)
    |> toEqual([%raw {| {a: 1, b: 2} |}])
  );
  test("fromKeys", () =>
    expect(M.fromKeys([|"hey", "there"|], S.length)) |> toEqual(M.pair("hey", 3, "there", 5))
  );
  test("fromKeysWith", () =>
    expect(M.fromKeysWith([|1, 2|], string_of_int, i => i + 2))
    |> toEqual(M.pair("1", 3, "2", 4))
  );
  test("fromKeyList", () =>
    expect(M.fromKeyList(["hey", "there"], S.length)) |> toEqual(M.pair("hey", 3, "there", 5))
  );
  test("makeFromSomes", () => {
    expect(
      M.makeFromSomes([|
        ("should have", Some(1)),
        ("should also have", Some(123)),
        ("should not have", None),
      |])
      |> M.toObject,
    )
    |> toEqual([%raw {| {'should have': 1, 'should also have': 123} |}])
  });
});

describe("merging", () => {
  let d1 = M.pair("a", 1, "b", 2);
  let d2 = M.pair("a", 2, "c", 3);

  test("override d1 with d2", () => {
    expect(d1 |> M.override(~values=d2) |> M.toObject)
    |> toEqual([%raw {| {a: 2, b: 2, c: 3} |}])
  });

  test("override d1 with d2 (no pipe operator)", () => {
    expect(M.override(d1, ~values=d2) |> M.toObject) |> toEqual([%raw {| {a: 2, b: 2, c: 3} |}])
  });

  test("override d1 with d2", () => {
    expect(d2 |> M.override(~values=d1) |> M.toObject)
    |> toEqual([%raw {| {a: 1, b: 2, c: 3} |}])
  });

  test("override d2 with d1", () => {
    expect(d2 |> M.override(~values=d1) |> M.toObject)
    |> toEqual([%raw {| {a: 1, b: 2, c: 3} |}])
  });

  test("mergeNoOverride d1 with d2", () => {
    expect(d1 |> M.mergeNoOverride(~values=d2) |> M.toObject)
    |> toEqual([%raw {| {a: 1, b: 2, c: 3} |}])
  });

  test("mergeNoOverride d2 with d1", () => {
    expect(d2 |> M.mergeNoOverride(~values=d1) |> M.toObject)
    |> toEqual([%raw {| {a: 2, b: 2, c: 3} |}])
  });
});

describe("adding keys", () => {
  let d = M.pair("a", 1, "b", 2);
  test("add a key with `set`", () => {
    expect(M.set(d, "c", 3) |> M.toObject) |> toEqual([%raw {| {a: 1, b: 2, c: 3} |}])
  });
  test("add a key with `add`", () => {
    expect(d |> M.add("e", 56) |> M.toObject) |> toEqual([%raw {| {a: 1, b: 2, e: 56} |}])
  });
});

describe("filtering", () => {
  let d = M.triple("x", 123, "y", 1000, "z", 9999);
  test("keep", () =>
    expect(M.keep(d, i => i > 500)) |> toEqual(M.pair("y", 1000, "z", 9999))
  );
  test("keepWithKey", () =>
    expect(M.keepWithKey(d, (k, i) => k == "x" || i == 9999))
    |> toEqual(M.pair("x", 123, "z", 9999))
  );
  test("keepMap (empty_", () =>
    expect(M.keepMap(d, _ => None)) |> toEqual(M.empty())
  );
  test("keepMap", () =>
    expect(M.keepMap(d, i => i > 200 ? None : Some(string_of_int(i))))
    |> toEqual(M.singleton("x", "123"))
  );
  test("keepMapWithKey", () =>
    expect(M.keepMapWithKey(d, (k, i) => k == "x" || i == 9999 ? Some(k) : None))
    |> toEqual(M.pair("x", "x", "z", "z"))
  );
  test("filter", () =>
    expect(d |> M.filter(i => i > 500)) |> toEqual(M.pair("y", 1000, "z", 9999))
  );
  test("filterWithKey", () =>
    expect(d |> M.filterWithKey((k, i) => k == "x" || i == 9999))
    |> toEqual(M.pair("x", 123, "z", 9999))
  );
});
