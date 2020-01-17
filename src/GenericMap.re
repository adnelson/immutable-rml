module L = Belt.List;
module A = Belt.Array;

module type Primitives = {
  type t('k, 'v);
  // Construction
  let fromArray: array(('k, 'v)) => t('k, 'v);
  let fromDict: Js.Dict.t('v) => t(string, 'v);
  let empty: unit => t('k, 'v);
};

// Functions implemented by all maps
module type GenericMap = {
  // Abstract type. Note that the key type is unrestricted here, but in practice
  // it must be something like a string or an integer to behave correctly.
  type t('k, 'v);
  exception IsEmpty;

  // Construction
  let fromArray: array(('k, 'v)) => t('k, 'v);
  let fromDict: Js.Dict.t('v) => t(string, 'v);
  let empty: unit => t('k, 'v);

  // Access
  let get: (t('k, 'v), 'k) => option('v);

  // Return the default if not found
  let getDefault: (t('k, 'v), 'k, 'v) => 'v;

  // Return the default if not found, lazily evaluated
  let getDefaultLazy: (t('k, 'v), 'k, unit => 'v) => 'v;

  // Raise a `NoSuchKey` if not found
  let getExn: (t('k, 'v), 'k) => 'v;

  // Raise the given exception if not found
  let getExnWith: (t('k, 'v), 'k, unit => exn) => 'v;

  // Alias to get with flipped order
  let take: ('k, t('k, 'v)) => option('v);

  // Alias to getExn with flipped order
  let takeExn: ('k, t('k, 'v)) => 'v;

  // Alias to getDefault with flipped order
  let takeDefault: ('k, 'v, t('k, 'v)) => 'v;

  // Alias to getDefaultLazy with flipped order
  let takeDefaultLazy: ('k, unit => 'v, t('k, 'v)) => 'v;

  let first: t('k, 'v) => option('v);
  let firstExn: t('k, 'v) => 'v;
  let firstKey: t('k, 'v) => option('k);
  let firstKeyExn: t('k, 'v) => 'k;
  let firstPair: t('k, 'v) => option(('k, 'v));
  let firstPairExn: t('k, 'v) => ('k, 'v);

  let filter: (t('k, 'v), ('v, 'k, t('k, 'v)) => bool) => t('k, 'v);

  // Operations
  let size: t('k, 'v) => int;
  let count: t('k, 'v) => int;
  let every: (t('k, 'v), ('v, 'k, t('k, 'v)) => bool) => bool;
  let has: (t('k, 'v), 'k) => bool;
  let isEmpty: t('k, 'v) => bool;
  let equals: (t('k, 'v), t('k, 'v)) => bool;

  // Transformation & iteration
  let forEach: (t('k, 'v), ('v, 'k, t('k, 'v)) => unit) => unit;
  let map: (t('k, 'v), ('v, 'k, t('k, 'v)) => 'v2) => t('k, 'v2);
  let each: ('v1 => 'v2, t('k, 'v1)) => t('k, 'v2);
  let eachPair: ('k1 => 'k2, 'v1 => 'v2, t('k1, 'v1)) => t('k2, 'v2);
  let reduce: (t('k, 'a), ('b, 'a, 'k, t('k, 'a)) => 'b, 'b) => 'b;
  let set: (t('k, 'v), 'k, 'v) => t('k, 'v);
  let sort: (t('k, 'v), ('v, 'v) => int) => t('k, 'v);

  // Conversions
  let toArray: t('k, 'v) => array(('k, 'v));
  let toObject: t(string, 'v) => Js.Dict.t('v);
  let toObjectWith: ('k => string, t('k, 'v)) => Js.Dict.t('v);
  let keys: t('k, 'v) => Js.Array.array_like('k);
  let keyArray: t('k, 'v) => array('k);
  let values: t('k, 'v) => Js.Array.array_like('v);
  let valueArray: t('k, 'v) => array('v);

  // Zip two orderedmaps which have the same key set, applying a function to each pair.
  let mergeWith: (t('k, 'a), ('a, 'b) => 'c, t('k, 'b)) => t('k, 'c);

  // Alias for mergeWith but with a different argument ordering.
  let zipWith: (t('k, 'a), t('k, 'b), ('a, 'b) => 'c) => t('k, 'c);

  // Invoke the given function on each key in the array to make an OrderedMap.
  let fromKeys: (array('k), 'k => 'v) => t('k, 'v);

  // Invoke the given function on each key in the array to make an OrderedMap.
  let fromKeyList: (list('k), 'k => 'v) => t('k, 'v);

  // Beginning with some initial map, create a new map with keys of the given
  // key array. If the key isn't in the initial map, the default is used.
  let hydrate: (array('k), 'v, t('k, 'v)) => t('k, 'v);

  // Beginning with some initial map, create a new map with keys of the given
  // key array. If the key isn't in the initial map, the generator function will
  // be called on the key to make a default.
  let hydrateWithKey: (array('k), 'k => 'v, t('k, 'v)) => t('k, 'v);

  // Beginning with some initial map, create a new map with keys of the given
  // key list. If the key isn't in the initial map, the generator function will
  // be called on the key to make a default.
  let hydrateList: (list('k), 'v, t('k, 'v)) => t('k, 'v);

  // Beginning with some initial map, create a new map with keys of the given
  // key array. If the key isn't in the initial map, the generator function will
  // be called on the key to make a default.
  let hydrateListWithKey: (list('k), 'k => 'v, t('k, 'v)) => t('k, 'v);

  // Apply a function to transform keys with the same order. The function must
  // produce a type that can be used as a key.
  let alterKeys: ('k1 => 'k2, t('k1, 'v)) => t('k2, 'v);

// `set` with different argument order
let add: ('k, 'v, t('k, 'v)) => t('k, 'v);

// Create a map with a single key and value
let singleton: ('k, 'v) => t('k, 'v);
};

// Given some primitive functions, makes everything else.
module MakeMap = (Prims: Primitives) : GenericMap => {
  include Prims;

  // Operations
  [@bs.get] external size: t('k, 'v) => int = "size";
  [@bs.send] [@bs.return nullable] external first: t('k, 'v) => option('v) = "first";
  [@bs.send] [@bs.return nullable] external get: (t('k, 'v), 'k) => option('v) = "get";
  [@bs.send] external keys: t('k, 'v) => Js.Array.array_like('k) = "keys";
  [@bs.send] external values: t('k, 'v) => Js.Array.array_like('v) = "values";
  [@bs.send] external count: t('k, 'v) => int = "count";
  [@bs.send] external every: (t('k, 'v), ('v, 'k, t('k, 'v)) => bool) => bool = "every";
  [@bs.send] external filter: (t('k, 'v), ('v, 'k, t('k, 'v)) => bool) => t('k, 'v) = "filter";
  [@bs.send] external forEach: (t('k, 'v), ('v, 'k, t('k, 'v)) => unit) => unit = "forEach";
  [@bs.send] external has: (t('k, 'v), 'k) => bool = "has";
  [@bs.send] external isEmpty: t('k, 'v) => bool = "isEmpty";
  [@bs.send] external map: (t('k, 'v), ('v, 'k, t('k, 'v)) => 'v2) => t('k, 'v2) = "map";
  [@bs.send] external reduce: (t('k, 'a), ('b, 'a, 'k, t('k, 'a)) => 'b, 'b) => 'b = "reduce";
  [@bs.send] external set: (t('k, 'v), 'k, 'v) => t('k, 'v) = "set";
  [@bs.send] external sort: (t('k, 'v), ('v, 'v) => int) => t('k, 'v) = "sort";
  [@bs.send] external toArray: t('k, 'v) => array(('k, 'v)) = "toArray";
  [@bs.send] external toObject: t(string, 'v) => Js.Dict.t('v) = "toObject";
  [@bs.send] external equals: (t('k, 'v), t('k, 'v)) => bool = "equals";
  [@bs.send]
  external mergeWith: (t('k, 'a), ('a, 'b) => 'c, t('k, 'b)) => t('k, 'c) = "mergeWith";

  let fromList = l => fromArray(L.toArray(l));

  // Thrown when an operation can't be completed because the map is empty.
  exception IsEmpty;

  // Raised when a key isn't found
  exception NoSuchKey;

  let keyArray: t('k, 'v) => array('k) = om => Js.Array.from(keys(om));
  let valueArray: t('k, 'v) => array('v) = om => Js.Array.from(values(om));

  let firstKey: t('k, 'v) => option('k) = om => A.get(keyArray(om), 0);

  let firstKeyExn: t('k, 'v) => 'k =
    om =>
      switch (A.get(keyArray(om), 0)) {
      | None => raise(IsEmpty)
      | Some(k) => k
      };

  let firstExn = om =>
    switch (first(om)) {
    | Some(x) => x
    | None => raise(IsEmpty)
    };

  let firstPair = om =>
    switch (first(om)) {
    | None => None
    | Some(responses) => Some((keyArray(om)[0], responses))
    };

  let firstPairExn: t('k, 'v) => ('k, 'v) =
    om =>
      switch (first(om)) {
      | None => raise(IsEmpty)
      | Some(responses) => (firstKeyExn(om), responses)
      };

  let toObjectWith = (toString, om) => {
    let pairs = toArray(om);
    Js.Dict.fromArray(pairs |> Array.map(((k, v)) => (toString(k), v)));
  };

  let zipWith = (om1, om2, f) => mergeWith(om1, f, om2);

  let getDefault = (om, k, default) =>
    switch (get(om, k)) {
    | None => default
    | Some(v) => v
    };

  let getDefaultLazy = (om, k, default) =>
    switch (get(om, k)) {
    | None => default()
    | Some(v) => v
    };

  let getExnWith = (om, k, mkExn) =>
    switch (get(om, k)) {
    | None => raise(mkExn())
    | Some(v) => v
    };

  let getExn = (om, k) =>
    getExnWith(
      om,
      k,
      () => {
        Js.log4("no such key", k, "of", keyArray(om));
        NoSuchKey;
      },
    );

  let take = (k, om) => get(om, k);
  let takeExn = (k, om) => getExn(om, k);
  let takeDefault = (k, default, om) => getDefault(om, k, default);
  let takeDefaultLazy = (k, default, om) => getDefaultLazy(om, k, default);

  // NOTE: I'm sure there's a primitive I could use here which would be more performant.
  let each = (f, om) => fromArray(A.map(toArray(om), ((k, v)) => (k, f(v))));

  // NOTE: I'm sure there's a primitive I could use here which would be more performant.
  let eachPair = (fk, fv, om) => fromArray(A.map(toArray(om), ((k, v)) => (fk(k), fv(v))));

  let fromKeys = (keys, f) => fromArray(A.map(keys, k => (k, f(k))));

  let hydrate = (arr, default, om) =>
    fromKeys(arr, k =>
      switch (get(om, k)) {
      | None => default
      | Some(v) => v
      }
    );

  let hydrateWithKey = (arr, gen, om) =>
    fromKeys(arr, k =>
      switch (get(om, k)) {
      | None => gen(k)
      | Some(v) => v
      }
    );

  let fromKeyList = (keys, f) => fromList(L.map(keys, k => (k, f(k))));

  let hydrateList = (keys) =>
    hydrate(keys |> Belt.List.toArray);

  let hydrateListWithKey = (keys) =>
    hydrateWithKey(keys |> Belt.List.toArray);

  let alterKeys: ('k1 => 'k2, t('k1, 'v)) => t('k2, 'v) =
    (f, om) => fromArray(A.map(toArray(om), ((k, v)) => (f(k), v)));

// `setPure` with different argument order
let add = (k, v, m) => set(m, k, v);

let singleton = (k, v) => fromArray([|(k, v)|]);
};
