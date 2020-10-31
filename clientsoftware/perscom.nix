{ stdenv, libusb-compat-0_1 }:

with stdenv.lib;

let
    name = "perscom-0.1";
in
stdenv.mkDerivation {
    inherit name;

    src = ./.;
    buildInputs = [ libusb-compat-0_1 ];

    installPhase = ''
        install -Dm 555 PERScom $out/bin/PERScom
    '';

    meta = {
        homepage = "https://github.com/con-f-use/perswitch";
        description = "Command line interface for home made power switch";
        license = stdenv.lib.licenses.mit;
        maintainers = [ "con-f-use <con-f-use@gmx.net>" ];
        platforms = stdenv.lib.platforms.linux;
    };
}
