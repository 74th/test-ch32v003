# ch32-hal の Cargo.toml を参考

https://github.com/ch32-rs/ch32-hal

マシンに一度だけ

```sh
rustup install nightly
```

```sh
cargo init --name test-ch2v003-rust-2

# nightlyを使う（2024/12/30現在）
rustup override set nightly
rustup component add rust-src
```

```toml
# .cargo/config.toml
[build]
# Target is specified in a file since RISCV32ec is not directly supported
target = "riscv32ec-unknown-none-elf.json"

[target.riscv32ec-unknown-none-elf]
# Use probe-rs for running the project and specify the linker script
# runner = "probe-rs run --chip ch32v003"
runner = "wlink -v flash --enable-sdi-print --watch-serial"

rustflags = ["-C", "link-arg=-Tlink.x"]

# Nightly Rust features for building a subset of the standard library
[unstable]
build-std = ["core"]
```

```sh
cargo add panic-halt
cargo add ch32-hal --git https://github.com/ch32-rs/ch32-hal --features ch32v003f4p6,memory-x
cargo add "qingke@*" --features highcode
cargo add "qingke-rt@*"
cargo add embedded-hal
```

## エラー

### ```error[E0463]: can't find crate for `core` ```

```
error[E0463]: can't find crate for `core`
  |
  = note: the `riscv32ec-unknown-none-elf` target may not be installed
  = help: consider downloading the target with `rustup target add riscv32ec-unknown-none-elf`
```

ngihtlyを利用していない。

```
rustup override set nightly
```

### error: failed to load bitcode of module "qingke-e06d6f12a36c3e8d.qingke.fb706560d3a52de4-cgu.0.rcgu.o":

わからん。Cargo.lockのhash違いで解決したりする。

ch32-halの以下の記述追加したら良かったのかも？

```
cargo add "qingke@*" --features highcode
cargo add "qingke-rt@*"
```

### qingke-rt-bbc0853b4f359ab8/out/link.x:1: cannot find linker script memory.x

```
  = note: some arguments are omitted. use `--verbose` to show all linker arguments
  = note: rust-lld: error: /home/nnyn/ghq/github.com/74th/test-ch32v003/20241230-rust-2/target/riscv32ec-unknown-none-elf/debug/build/qingke-rt-bbc0853b4f359ab8/out/link.x:1: cannot find linker script memory.x
          >>> INCLUDE memory.x
```

ch32-hal の features に memory-x が足りてない
