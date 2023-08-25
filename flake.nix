{
  description = "Structural Simulation Toolkit (SST) Parallel Discrete Event Core";
  inputs.nixpkgs.url = "nixpkgs/nixos-23.05";
  outputs = { self, nixpkgs }:
    let
      # to work with older version of flakes
      lastModifiedDate = self.lastModifiedDate or self.lastModified or "19700101";
      # Generate a user-friendly version number.
      version = "13.0.0";
      supportedSystems = [
        "x86_64-linux"
        "x86_64-darwin"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      # Helper function to generate an attrset '{ x86_64-linux = f "x86_64-linux"; ... }'.
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      # Nixpkgs instantiated for supported system types.
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlay ]; });
    in
    {
      # A Nixpkgs overlay.
      overlay = final: prev: {
        sst-core = with final; stdenv.mkDerivation rec {
          pname = "sst-core";
          inherit version;
          src = ./.;
          nativeBuildInputs = [
            autoreconfHook
            # autoconf
            # automake
            # gettext
            # libtool
          ];
          buildInputs = [
            mpi
            python3Full
          ];
          # preConfigure = ''
          #   ./autogen.sh
          # '';
          # configure = ''
          #   ./configure \
          #     --prefix=$out
          # '';
          # buildPhase = ''
          #   make all
          # '';
          # installPhase = ''
          #   make install
          # '';
        };
      };

      # Provide some binary packages for selected system types.
      packages = forAllSystems (system:
        {
          inherit (nixpkgsFor.${system}) sst-core;
        }
      );

      # The default package for 'nix build'. This makes sense if the
      # flake provides only one package or there is a clear "main"
      # package.
      defaultPackage = forAllSystems (system: self.packages.${system}.sst-core);

      # Tests run by 'nix flake check' and by Hydra.
      # checks = forAllSystems
      #   (system:
      #     with nixpkgsFor.${system};
      #     {
      #       inherit (self.packages.${system}) hello;
      #       # Additional tests, if applicable.
      #       test = stdenv.mkDerivation {
      #         pname = "sst-core-test";
      #         inherit version;
      #         buildInputs = [ sst-core ];
      #         dontUnpack = true;
      #         buildPhase = ''
      #           echo 'running some integration tests'
      #           [[ $(hello) = 'Hello Nixers!' ]]
      #         '';
      #         installPhase = "mkdir -p $out";
      #       };
      #     }
      #     // lib.optionalAttrs stdenv.isLinux {
      #       # A VM test of the NixOS module.
      #       vmTest =
      #         with import (nixpkgs + "/nixos/lib/testing-python.nix") {
      #           inherit system;
      #         };
      #         makeTest {
      #           nodes = {
      #             client = { ... }: {
      #               imports = [ self.nixosModules.hello ];
      #             };
      #           };
      #           testScript =
      #             ''
      #               start_all()
      #               client.wait_for_unit("multi-user.target")
      #               client.succeed("hello")
      #             '';
      #         };
      #     }
      #   );
    };
}
