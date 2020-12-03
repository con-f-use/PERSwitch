{ stdenv, libusb-compat-0_1 }:

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

    meta = with stdenv.lib {
        homepage = "https://github.com/con-f-use/perswitch";
        description = "Command line interface for home made power switch";
        license = licenses.mit;
        maintainers = [ maintainers.confus ];
        platforms = platforms.linux;
    };
}
