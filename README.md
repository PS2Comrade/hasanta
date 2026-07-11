
# hasanta

<img src="https://github.com/PS2Comrade/picx-images-hosting/raw/master/hasanta.1lcig1bvkv.webp" width="150">

 **hasanta** (হসন্ত) - abugida for **nix* in mind
built from scratch to handle bengali input for fcitx5, thus making it cross compatible with any platform supporting the fcitx5 framework.

# **features :-**

 - fast - built with bare c++ without any external dependency
 - support for preview window to preview typed text under the cursor
 - compatibility - work in progress avro phonetic and bijay scheme 
 - built from scratch - this isn't a fork, its made from ground up with avro support being reversed engineered
 
 # **installation:-**
 
 prerequisites:
 
    sudo pacman -S base-devel git ninja meson fcitx5
**build:-**

    git clone https://github.com/PS2Comrade/hasanta.git && cd hasanta && meson setup build --prefix=/usr && meson compile -C build

**install to system:-**

    sudo meson install -C build

# **usage:-**

 1. restart your fcitx5 daemon: `killall fcitx5 && fcitx5 -d &`
 2. open `fcitx5-configtool`
 3. uncheck "Only Show Current Language"
 4. Search for `hasanta` and add it to your active input methods

# **license:-**
this project is licensed under GPL-3.0
<img src="https://pillbanana.com/wp-content/uploads/2018/07/gpl_generalpubliclicense_logo.png" width="150" align="right" alt="GPL-3.0">


