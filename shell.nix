with import <nixpkgs> { };
mkShell {
  nativeBuildInputs = with pkgs; [ raylib pkg-config cmake meson ninja gcc ];
}
