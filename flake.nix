{
  description =
    "A program to edit Konsole config files (konsolerc) instead of the UI";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    (flake-utils.lib.eachDefaultSystem (system:
      let pkgs = import nixpkgs { inherit system; };
      in rec {
        packages.home-manager-konsole =
          pkgs.callPackage ./home-manager-konsole.nix { };
        packages.default = self.outputs.packages.${system}.home-manager-konsole;
        devShell = pkgs.mkShell {
          inputsFrom = [ packages.default ];
          packages = with pkgs; [ clang-tools ];
        };
      }));
}
