// ReasonML interface to immutable.js OrderedMap type

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
// Raise a `NoSuchKey` if not found
let getExn: (t('k, 'v), 'k) => 'v;
// Raise the given exception if not found
let getExnWith: (t('k, 'v), 'k, unit => exn) => 'v;

// Alias to getExn with flipped order
let takeExn: ('k, t('k, 'v)) => 'v;
let takeDefault: ('k, 'v, t('k, 'v)) => 'v;

let first: t('k, 'v) => option('v);
let firstExn: t('k, 'v) => 'v;
let firstKey: (t('k, 'v)) => option('k);
let firstKeyExn: (t('k, 'v)) => 'k;
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

// Beginning with some iniital map, create a new map with keys of the given
// key array. If the key isn't in the initial map, the generator function will
// be called on the key to make a default.
let hydrate: (t('k, 'v), array('k), 'k => 'v) => t('k, 'v);

// Beginning with some iniital map, create a new map with keys of the given
// key list. If the key isn't in the initial map, the generator function will
// be called on the key to make a default.
let hydrateList: (t('k, 'v), list('k), 'k => 'v) => t('k, 'v);
