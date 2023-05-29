{ lib, stdenv, cmake, libsForQt5, extra-cmake-modules }:
stdenv.mkDerivation {
  pname = "home-manager-konsole";
  version = "1.0";
  src = ./.;
  buildInputs = with libsForQt5; [ kconfig kxmlgui ];
  nativeBuildInputs =
    [ cmake extra-cmake-modules libsForQt5.qt5.wrapQtAppsHook ];
}
