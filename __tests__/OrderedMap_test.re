module OM = Map.OrderedMap;
open Jest;
open Expect;

module D = Js.Dict;
module S = Js.String;
module A = Belt.Array;

describe("mapping", () => {
  let omap =
    [|
      ("A", "Candy Bars"),
      ("B", "Peanut Butter Cups"),
      ("C", "Ice Cream"),
    |]
    |> OM.fromArray;

  describe("map (data structure first argument)", () => {
    test("map values only", () => expect(OM.map(omap, S.length) |> OM.valueArray) |> toEqual([|10, 18, 9|]));
    test("map with key", () => expect(OM.mapWithKey(omap, (k, s) => k ++ s) |> OM.valueArray) |> toEqual([|"ACandy Bars", "BPeanut Butter Cups", "CIce Cream"|]));
  });
  describe("each (data structure last argument)", () => {
  test("each values only", () => expect(omap |> OM.each(S.length) |> OM.valueArray) |> toEqual([|10, 18, 9|]));
    test("each with key", () => expect(omap |> OM.eachWithKey((k, s) => k ++ s) |> OM.valueArray) |> toEqual([|"ACandy Bars", "BPeanut Butter Cups", "CIce Cream"|]));
});
});
