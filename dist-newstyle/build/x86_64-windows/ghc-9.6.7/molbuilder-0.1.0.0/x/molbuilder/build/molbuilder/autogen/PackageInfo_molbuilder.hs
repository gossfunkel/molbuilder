{-# LANGUAGE NoRebindableSyntax #-}
{-# OPTIONS_GHC -fno-warn-missing-import-lists #-}
{-# OPTIONS_GHC -w #-}
module PackageInfo_molbuilder (
    name,
    version,
    synopsis,
    copyright,
    homepage,
  ) where

import Data.Version (Version(..))
import Prelude

name :: String
name = "molbuilder"
version :: Version
version = Version [0,1,0,0] []

synopsis :: String
synopsis = "molecule builder game"
copyright :: String
copyright = ""
homepage :: String
homepage = ""
