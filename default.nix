{
  # Per default, we should build against a stable version of nixpkgs
  # Note: Not using fetchFromGitHub because it comes from nixpkgs and we would import two versions
  # pkgs ? import (builtins.fetchTarball {
  #   name = "nixos-stable-20.09";
  #   url = "https://github.com/NixOS/nixpkgs/archive/5272327b81ed355bbed5659b8d303cf2979b6953.tar.gz";
  #   sha256 = "0182ys095dfx02vl2a20j1hz92dx3mfgz2a6fhn31bqlp1wa8hlq";
  # }) {}
  pkgs ? import <nixpkgs> {}
}:

pkgs.lib.makeExtensible (self: rec {
  inherit pkgs;

  perscom = pkgs.callPackage ./clientsoftware/perscom.nix {
    #xfce4-power-manager = pkgs.xfce.xfce4-power-manager;
  };
  #persgui = ...;
} )

