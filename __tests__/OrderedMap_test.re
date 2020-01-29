module M = Map.OrderedMap;

// Test all of the generic functionality
include GenericMapTests.GenericMapTests(M);

// Test ordering-specific functions addition to the generic map tests.
Jest.describe("ordering", () => {
  open Jest;
  open Expect;

  let map =
    [|("A", "Candy Bars"), ("B", "Peanut Butter Cups"), ("C", "Ice Cream")|] |> M.fromArray;
  let empty = M.empty();

  test("first", () =>
    expect(M.first(map)) |> toEqual(Some("Candy Bars"))
  );
  test("firstKey", () =>
    expect(M.firstKey(map)) |> toEqual(Some("A"))
  );
  test("firstPair", () =>
    expect(M.firstPair(map)) |> toEqual(Some(("A", "Candy Bars")))
  );
  test("last", () =>
    expect(M.last(map)) |> toEqual(Some("Ice Cream"))
  );
  test("lastKey", () =>
    expect(M.lastKey(map)) |> toEqual(Some("C"))
  );
  test("lastPair", () =>
    expect(M.lastPair(map)) |> toEqual(Some(("C", "Ice Cream")))
  );

  test("first when empty", () =>
    expect(M.first(empty)) |> toEqual(None)
  );
  test("firstKey when empty", () =>
    expect(M.firstKey(empty)) |> toEqual(None)
  );
  test("firstPair when empty", () =>
    expect(M.firstPair(empty)) |> toEqual(None)
  );
  test("last when empty", () =>
    expect(M.last(empty)) |> toEqual(None)
  );
  test("lastKey when empty", () =>
    expect(M.lastKey(empty)) |> toEqual(None)
  );
  test("lastPair when empty", () =>
    expect(M.lastPair(empty)) |> toEqual(None)
  );

  describe("exception-throwing", () => {
    test("firstExn", () =>
      expect(M.firstExn(map)) |> toEqual("Candy Bars")
    );
    test("firstKeyExn", () =>
      expect(M.firstKeyExn(map)) |> toEqual("A")
    );
    test("firstPairExn", () =>
      expect(M.firstPairExn(map)) |> toEqual(("A", "Candy Bars"))
    );
    test("lastExn", () =>
      expect(M.lastExn(map)) |> toEqual("Ice Cream")
    );
    test("lastKeyExn", () =>
      expect(M.lastKeyExn(map)) |> toEqual("C")
    );
    test("lastPairExn", () =>
      expect(M.lastPairExn(map)) |> toEqual(("C", "Ice Cream"))
    );
  });
});
