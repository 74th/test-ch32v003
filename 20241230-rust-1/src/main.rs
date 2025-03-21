#![no_std]
#![no_main]

use ch32_hal as hal;
use hal::delay::Delay;
use hal::gpio::{Level, Output};
use panic_halt as _;

#[qingke_rt::entry]
fn main() -> ! {
    hal::debug::SDIPrint::enable();
    let config = hal::Config::default();
    let peripherals = hal::init(config);

    let mut led = Output::new(peripherals.PD6, Level::Low, Default::default());

    let mut delay = Delay;

    loop {
        led.toggle();
        delay.delay_ms(1000);
        hal::println!("toggle!");
    }
}
