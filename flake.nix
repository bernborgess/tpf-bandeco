{
  description = "C++ SDL2 development flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages."x86_64-linux";
    in
    {
      devShells."x86_64-linux".default = pkgs.mkShell {
        buildInputs = with pkgs; [
          # C++ tools
          cmake
          gdb
          ninja

          # SDL2 Libraries
          SDL2
          SDL2_ttf
          libtiff
          SDL2_image
          libwebp
          SDL2_mixer
          libsndfile
        ];
        nativeBuildInputs = [ pkgs.pkg-config ];
      };
    };
}
