// Unordered map
module Map: GenericMap.GenericMap =
  GenericMap.MakeMap({
    // Abstract
    type t('k, 'v);

    // Construction
    [@bs.module "immutable"] external fromArray: array(('k, 'v)) => t('k, 'v) = "Map";
    [@bs.module "immutable"] external fromDict: Js.Dict.t('v) => t(string, 'v) = "Map";
    [@bs.module "immutable"] external empty: unit => t('k, 'v) = "Map";
  });

// Ordered map data structure
module OrderedMap: GenericMap.GenericMap =
  GenericMap.MakeMap({
    // Abstract
    type t('k, 'v);

    // Construction
    [@bs.module "immutable"] external fromArray: array(('k, 'v)) => t('k, 'v) = "OrderedMap";
    [@bs.module "immutable"] external fromDict: Js.Dict.t('v) => t(string, 'v) = "OrderedMap";
    [@bs.module "immutable"] external empty: unit => t('k, 'v) = "OrderedMap";
  });
