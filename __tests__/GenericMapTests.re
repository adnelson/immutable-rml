open GenericMap;
open Jest;
open Expect;
module S = String;

// Creates a test suite for a generic map
module GenericMapTests = (M: GenericMap) => {
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
      expect(M.fromKeys([|"hey", "there"|], S.length))
      |> toEqual(M.pair("hey", 3, "there", 5))
    );
    test("fromKeysWith", () =>
      expect(M.fromKeysWith([|1, 2|], string_of_int, i => i + 2))
      |> toEqual(M.pair("1", 3, "2", 4))
    );
    test("fromKeyList", () =>
      expect(M.fromKeyList(["hey", "there"], S.length))
      |> toEqual(M.pair("hey", 3, "there", 5))
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

  describe("size", () => {
    test("empty", () =>
      expect(M.size(M.empty())) |> toEqual(0)
    );
    test("single", () =>
      expect(M.size(M.singleton("hello", 1))) |> toEqual(1)
    );
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

  describe("merging", () => {
    let d1 = M.pair("a", 1, "b", 2);
    let d2 = M.pair("a", 2, "c", 3);

    test("override d1 with d2", () => {
      expect(d1 |> M.override(~values=d2) |> M.toObject)
      |> toEqual([%raw {| {a: 2, b: 2, c: 3} |}])
    });

    test("override d1 with d2 (no pipe operator)", () => {
      expect(M.override(d1, ~values=d2) |> M.toObject)
      |> toEqual([%raw {| {a: 2, b: 2, c: 3} |}])
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

  describe("removing keys", () => {
    let d = M.pair("a", 1, "b", 2);
    test("remove a key with `delete`", () => {
      expect(M.delete(d, "b") |> M.toObject) |> toEqual([%raw {| {a: 1} |}])
    });
    test("remove a key with `remove`", () => {
      expect(d |> M.remove("a") |> M.toObject) |> toEqual([%raw {| {b: 2} |}])
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

  describe("all/any", () => {
      let m = M.triple("a", 1, "b", 2, "c", 3);
      test("all even = false", () => expect(m |> M.all(i => i mod 2 == 0)) |> toBe(false));
      test("all > 0 = true", () => expect(m |> M.all(i => i > 0)) |> toBe(true));
      test("any even = true", () => expect(m |> M.any(i => i mod 2 == 0)) |> toBe(true));
      test("any > 0 = true", () => expect(m |> M.any(i => i > 0)) |> toBe(true));
    describe("with key", () => {
      test("all false", () => expect(m |> M.allWithKey((k, i) => k == "a" && i mod 2 == 0)) |> toBe(false));
      test("all true", () => expect(m |> M.allWithKey((k, i) => k == "a" || i > 1)) |> toBe(true));
      test("any false", () => expect(m |> M.anyWithKey((k, _) => S.length(k) > 1)) |> toBe(false));
      test("any true", () => expect(m |> M.anyWithKey((k, i) => k == "a" || i > 1)) |> toBe(true));});
  });
};
