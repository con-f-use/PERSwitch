{ stdenv, lib, libusb-compat-0_1 }:

stdenv.mkDerivation {
    pname = "perscom";
    version = "0.1";

    src = ./..;
    buildInputs = [ libusb-compat-0_1 ];

    buildPhase = ''
        cd clientsoftware
        make PERScom
    '';

    installPhase = ''
        install -Dm 555 PERScom $out/bin/PERScom
    '';

    meta = {
        homepage = "https://github.com/con-f-use/perswitch";
        description = "Command line interface for home made power switch";
        license = lib.licenses.mit;
        maintainers = [ lib.maintainers.confus ];
        platforms = lib.platforms.linux;
    };
}
