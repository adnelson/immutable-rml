module L = Belt.List;
module A = Belt.Array;

// Functions implemented by all maps
module type GenericMap = {
  // Abstract type. Note that the key type is unrestricted here, but in practice
  // it must be something like a string or an integer to behave correctly.
  type t('k, 'v);
  exception IsEmpty;

  // Construction
  let fromArray: array(('k, 'v)) => t('k, 'v);
  let fromList: list(('k, 'v)) => t('k, 'v);
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

  // Filter out objects by value.
  let keep: (t('k, 'v), 'v => bool) => t('k, 'v);

  // Filter out objects by key and value.
  let keepWithKey: (t('k, 'v), ('k, 'v) => bool) => t('k, 'v);

  // Same as `keep` with reverse argument order.
  let filter: ('v => bool, t('k, 'v)) => t('k, 'v);

  // Same as `keepWithKey` with reverse argument order.
  let filterWithKey: (('k, 'v) => bool, t('k, 'v)) => t('k, 'v);

  // Get the size of the map.
  let size: t('k, 'v) => int;

  // Returns true if every value in the map satisfies the predicate.
  let every: (t('k, 'v), 'v => bool) => bool;

  // Like `every` but also has access to the key.
  let everyWithKey: (t('k, 'v), ('k, 'v) => bool) => bool;

  // Same as `every` but reversed argument order.
  let all: ('v => bool, t('k, 'v)) => bool;

  // Like `all` but also has access to the key.
  let allWithKey: (('k, 'v) => bool, t('k, 'v)) => bool;

  // Returns true if some value in the map satisfies the predicate.
  let some: (t('k, 'v), 'v => bool) => bool;

  // Like `some` but also has access to the key.
  let someWithKey: (t('k, 'v), ('k, 'v) => bool) => bool;

  // Same as `some` but reversed argument order.
  let any: ('v => bool, t('k, 'v)) => bool;

  // Like `any` but also has access to the key.
  let anyWithKey: (('k, 'v) => bool, t('k, 'v)) => bool;

  // Check if a key is in the map.
  let has: (t('k, 'v), 'k) => bool;

  // Check if the map is empty.
  let isEmpty: t('k, 'v) => bool;

  // Check if the map is equivalent to another map.
  let equals: (t('k, 'v), t('k, 'v)) => bool;

  // Perform a side effect on every value.
  let forEach: (t('k, 'v), 'v => unit) => unit;

  // Perform a side effect on every key/value.
  let forEachWithKey: (t('k, 'v), ('k, 'v) => unit) => unit;

  // Map a function over the values in a map.
  let map: (t('k, 'v1), 'v1 => 'v2) => t('k, 'v2);

  // Map a function over the values in a map also having access to the keys.
  let mapWithKey: (t('k, 'v1), ('k, 'v1) => 'v2) => t('k, 'v2);

  // Like `mapWithKey` but can change the key type.
  let mapPairs: (t('k1, 'v1), 'k1 => 'k2, 'v1 => 'v2) => t('k2, 'v2);

  // Like `map` with reverse argument order.
  let each: ('v1 => 'v2, t('k, 'v1)) => t('k, 'v2);

  // Like `mapWithKey` with reverse argument order.
  let eachWithKey: (('k, 'v1) => 'v2, t('k, 'v1)) => t('k, 'v2);

  // Like `mapPair` with reverse argument order.
  let eachPair: ('k1 => 'k2, 'v1 => 'v2, t('k1, 'v1)) => t('k2, 'v2);

  // Reduce the elements in the map into some result.
  let reduce: (t('k, 'a), 'b, ('a, 'b) => 'b) => 'b;

  // Like `reduce` but has access to the keys.
  let reduceWithKey: (t('k, 'a), 'b, ('k, 'a, 'b) => 'b) => 'b;

  // Like `reduce` with different argument order.
  let fold: ('b, ('a, 'b) => 'b, t('k, 'a)) => 'b;

  // Like `reduce` but has access to the keys.
  let foldWithKey: ('b, ('k, 'a, 'b) => 'b, t('k, 'a)) => 'b;

  // Add a key to the map.
  let set: (t('k, 'v), 'k, 'v) => t('k, 'v);

  // Like `set` with reverse argument order.
  let insert: ('k, 'v, t('k, 'v)) => t('k, 'v);

  // Conversions
  let toArray: t('k, 'v) => array(('k, 'v));
  let toList: t('k, 'v) => list(('k, 'v));
  let toObject: t(string, 'v) => Js.Dict.t('v);
  let toObjectWith: ('k => string, t('k, 'v)) => Js.Dict.t('v);
  let keys: t('k, 'v) => Js.Array.array_like('k);
  let keyArray: t('k, 'v) => array('k);
  let keyList: t('k, 'v) => list('k);
  let values: t('k, 'v) => Js.Array.array_like('v);
  let valueArray: t('k, 'v) => array('v);
  let valueList: t('k, 'v) => list('v);

  // Zip two orderedmaps which have the same key set, applying a function to each pair.
  let mergeWith: (t('k, 'a), t('k, 'b), ('a, 'b) => 'c) => t('k, 'c);

  // Alias for mergeWith but with a different argument ordering.
  let zipWith: (t('k, 'b), ('a, 'b) => 'c, t('k, 'a)) => t('k, 'c);

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

  // Create a map with a single key and value
  let singleton: ('k, 'v) => t('k, 'v);
};

module type GenericOrderedMap = {
  include GenericMap;

  // Get the first value out of a map.
  let first: t('k, 'v) => option('v);

  // Get the first value out of a map, throwing an exception if empty.
  let firstExn: t('k, 'v) => 'v;

  // Get the first key out of a map.
  let firstKey: t('k, 'v) => option('k);

  // Get the first key out of a map, throwing an exception if empty.
  let firstKeyExn: t('k, 'v) => 'k;

  // Get the first key/value pair out of a map.
  let firstPair: t('k, 'v) => option(('k, 'v));

  // Get the first key/value pair out of a map, throwing an exception if empty.
  let firstPairExn: t('k, 'v) => ('k, 'v);

  // Sort the ordered map by a comparison on values.
  let sort: (t('k, 'v), ('v, 'v) => int) => t('k, 'v);
};

module type Primitives = {
  type t('k, 'v);
  // Construction
  let fromArray: array(('k, 'v)) => t('k, 'v);
  let fromDict: Js.Dict.t('v) => t(string, 'v);
  let empty: unit => t('k, 'v);
};

// Given some primitive functions, makes everything else.
module MakeMap = (Prims: Primitives) : GenericMap => {
  include Prims;

  // Operations
  [@bs.get] external size: t('k, 'v) => int = "count";
  [@bs.send] [@bs.return nullable] external get: (t('k, 'v), 'k) => option('v) = "get";
  [@bs.send] external keys: t('k, 'v) => Js.Array.array_like('k) = "keys";
  [@bs.send] external values: t('k, 'v) => Js.Array.array_like('v) = "values";
  [@bs.send] external every: (t('k, 'v), 'v => bool) => bool = "every";
  [@bs.send] external everyWithKey_: (t('k, 'v), ('v, 'k) => bool) => bool = "every";
  [@bs.send] external some: (t('k, 'v), 'v => bool) => bool = "some";
  [@bs.send] external someWithKey_: (t('k, 'v), ('v, 'k) => bool) => bool = "some";
  [@bs.send] external filter_: (t('k, 'v), ('v, 'k) => bool) => t('k, 'v) = "filter";
  [@bs.send] external forEach: (t('k, 'v), 'v => unit) => unit = "forEach";
  [@bs.send] external forEachWithKey_: (t('k, 'v), ('v, 'k) => unit) => unit = "forEach";
  [@bs.send] external has: (t('k, 'v), 'k) => bool = "has";
  [@bs.send] external isEmpty: t('k, 'v) => bool = "isEmpty";
  [@bs.send] external map: (t('k, 'v1), 'v1 => 'v2) => t('k, 'v2) = "map";
  [@bs.send] external mapWithKey_: (t('k, 'v1), ('v1, 'k) => 'v2) => t('k, 'v2) = "map";
  [@bs.send] external reduce_: (t('k, 'a), ('b, 'a, 'k) => 'b, 'b) => 'b = "reduce";
  [@bs.send] external set: (t('k, 'v), 'k, 'v) => t('k, 'v) = "set";
  [@bs.send] external toArray: t('k, 'v) => array(('k, 'v)) = "toArray";
  [@bs.send] external toObject: t(string, 'v) => Js.Dict.t('v) = "toObject";
  [@bs.send] external equals: (t('k, 'v), t('k, 'v)) => bool = "equals";
  [@bs.send]
  external mergeWith_: (t('k, 'a), ('a, 'b) => 'c, t('k, 'b)) => t('k, 'c) = "mergeWith";

  let fromList = l => fromArray(L.toArray(l));

  // Thrown when an operation can't be completed because the map is empty.
  exception IsEmpty;

  // Raised when a key isn't found
  exception NoSuchKey;

  let keep = (m, pred) => filter_(m, (v, _) => pred(v));
  let keepWithKey = (m, pred) => filter_(m, (v, k) => pred(k, v));
  let filter = (pred, m) => filter_(m, (v, _) => pred(v));
  let filterWithKey = (pred, m) => filter_(m, (v, k) => pred(k, v));
  let toList = m => m |> toArray |> L.fromArray;
  let keyArray: t('k, 'v) => array('k) = om => Js.Array.from(keys(om));
  let keyList = m => m |> keyArray |> L.fromArray;
  let valueArray: t('k, 'v) => array('v) = om => Js.Array.from(values(om));
  let valueList = m => m |> valueArray |> L.fromArray;
  let reduce = (m, start, f) => reduce_(m, (out, v, _) => f(v, out), start);
  let reduceWithKey = (m, start, f) => reduce_(m, (out, v, k) => f(k, v, out), start);
  let fold = (start, f, m) => reduce_(m, (out, v, _) => f(v, out), start);
  let foldWithKey = (start, f, m) => reduce_(m, (out, v, k) => f(k, v, out), start);
  let everyWithKey = (m, pred) => everyWithKey_(m, (v, k) => pred(k, v));
  let all = (pred, m) => every(m, pred);
  let allWithKey = (pred, m) => everyWithKey_(m, (v, k) => pred(k, v));
  let someWithKey = (m, pred) => someWithKey_(m, (v, k) => pred(k, v));
  let any = (pred, m) => some(m, pred);
  let anyWithKey = (pred, m) => someWithKey_(m, (v, k) => pred(k, v));
  let forEachWithKey = (m, f) => forEachWithKey_(m, (v, k) => f(k, v));
  let mergeWith = (m1, m2, f) => mergeWith_(m1, f, m2);
  let zipWith = (m2, f, m1) => mergeWith_(m1, f, m2);
  let mapWithKey = (om, f) => mapWithKey_(om, (v, k) => f(k, v));
  let eachWithKey = (f, om) => mapWithKey_(om, (v, k) => f(k, v));
  let each = (f, om) => map(om, f);

  let getDefaultLazy = (om, k, default) =>
    switch (get(om, k)) {
    | None => default()
    | Some(v) => v
    };

  let getDefault = (om, k, default) =>
    switch (get(om, k)) {
    | None => default
    | Some(v) => v
    };

  let getExnWith = (om, k, mkExn) =>
    switch (get(om, k)) {
    | None => raise(mkExn())
    | Some(v) => v
    };

  let getExn = (om, k) => getExnWith(om, k, () => NoSuchKey);

  let take = (k, om) => get(om, k);
  let takeExn = (k, om) => getExn(om, k);
  let takeDefault = (k, default, om) => getDefault(om, k, default);
  let takeDefaultLazy = (k, default, om) => getDefaultLazy(om, k, default);

  // NOTE: for eachPair and mapPairs there might be a primitive
  let eachPair = (fk, fv, om) => fromArray(A.map(toArray(om), ((k, v)) => (fk(k), fv(v))));
  let mapPairs = (om, fk, fv) => fromArray(A.map(toArray(om), ((k, v)) => (fk(k), fv(v))));

  let toObjectWith = (toString, om) => {
    let pairs = toArray(om);
    Js.Dict.fromArray(pairs |> Array.map(((k, v)) => (toString(k), v)));
  };
  let fromKeys = (keys, f) => fromArray(A.map(keys, k => (k, f(k))));
  let fromKeyList = (keys, f) => fromList(L.map(keys, k => (k, f(k))));

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

  let hydrateList = keys => hydrate(keys |> Belt.List.toArray);
  let hydrateListWithKey = keys => hydrateWithKey(keys |> Belt.List.toArray);
  let alterKeys = f => eachPair(f, v => v);
  let insert = (k, v, m) => set(m, k, v);
  let singleton = (k, v) => fromArray([|(k, v)|]);
};

module MakeOrderedMap = (Prims: Primitives) : GenericOrderedMap => {
  include MakeMap(Prims);
  [@bs.send] [@bs.return nullable] external first: t('k, 'v) => option('v) = "first";
  [@bs.send] external sort: (t('k, 'v), ('v, 'v) => int) => t('k, 'v) = "sort";

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
};
