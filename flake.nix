{
  description = "litcode: terminal DSA (TypeScript), systems (C + Rust) and Postgres training";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }:
    let
      systems = [ "aarch64-darwin" "x86_64-darwin" "x86_64-linux" "aarch64-linux" ];
      forAll = f: nixpkgs.lib.genAttrs systems (system: f nixpkgs.legacyPackages.${system});
    in
    {
      devShells = forAll (pkgs: {
        # clangStdenv: `cc` is clang (with ASan/UBSan runtimes), never gcc.
        default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          packages = with pkgs; [
            # TypeScript track
            bun
            nodejs_22
            typescript-language-server
            # C + Rust track
            gnumake
            clang-tools # clangd, clang-format
            cargo
            rustc
            clippy
            rustfmt
            rust-analyzer
            # SQL track
            postgresql_17 # psql client (the server runs in docker: lc sql up)
            # runner
            watchexec
            glow
            bat
          ] ++ lib.optionals stdenv.hostPlatform.isLinux [ valgrind gdb ];

          shellHook = ''
            export PATH="$PWD:$PATH"
            echo "litcode dev shell — run: lc help"
          '';
        };
      });
    };
}
