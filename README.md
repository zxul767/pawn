# Pawn
![status](https://github.com/zxul767/pawn/actions/workflows/build.yml/badge.svg)

A chess engine and machine learning experimentation testbed.

Originally written as a project to investigate AI techniques, `pawn` is now at a decent strength to beat average players. It has been tested against some popular enginess (such as GnuChess) as part of its training to evolve with some satisfactory results.

I will be adding details about its architecture and the research behind it later on.

## Quick Start
```bash
make && bin/pawn
```

<img width="709" alt="Screen Shot 2022-03-09 at 15 01 28" src="https://user-images.githubusercontent.com/442314/157535523-15d8655e-0ba4-4fa0-9d8f-2198d8cc8925.png">

If you'd rather use a GUI, you can run:
```bash
make && xboard -fcp bin/pawn
```
but you'll need to have [`xboard`](https://www.gnu.org/software/xboard/) pre-installed as `pawn` doesn't have a GUI of its own.

<img width="713" alt="Screen Shot 2022-03-09 at 15 04 31" src="https://user-images.githubusercontent.com/442314/157535930-b36ca86b-532f-4c73-b00a-eb81f7adb7c0.png">

If you find issues running any of the above, check the following sections for installation details.

## Dependencies
### Build Tools
+ A C++20-capable compiler, such as [`gcc`](https://gcc.gnu.org)
+ [`make`](https://www.gnu.org/software/make/)
+ [`fmt`](https://fmt.dev/) development headers and library
+ [`uv`](https://docs.astral.sh/uv/) for Python development tools

In Debian-based distributions, you should be able to install these by running:

```bash
sudo apt-get install build-essential libfmt-dev
```

See [this post](https://www.cyberciti.biz/faq/debian-linux-install-gnu-gcc-compiler/) for detailed instructions on installing GNU compiler tools.

If you're using Mac OS X, both tools should be available in your terminal if you've installed `XCode`; otherwise, you can follow the instructions in [this post](https://www.freecodecamp.org/news/install-xcode-command-line-tools/) to install them.

To install `fmt` on Mac OS X:

```bash
brew install fmt
```

After installing `uv`, sync the Python development tools with:

```bash
uv sync --dev
```

This installs tools such as `pre-commit` and the compact test reporter's `rich` dependency into the project virtual environment.

To install the Git pre-commit hook from that environment:

```bash
uv run pre-commit install
```

`pre-commit` does not need to be installed globally. The Git hook can call the copy installed in the project virtual environment.

### Graphical Interface (Optional)
+ [`xboard`](https://www.gnu.org/software/xboard/)

To install it in Linux or other Debian-based distributions, run the following command in your terminal:

```bash
sudo apt install xboard
```

To install it in Mac OS X:

```bash
brew install xboard
```

Notice that only a very small subset of functionality available from the GUI has been implemented in the engine.

## Console Mode
If you don't want to use the GUI, you can still play using the console by running:

```bash
bin/pawn
```

In this case, you can make moves by typing the initial and final square of any piece on the board. For example, you can type `e2e4` to move the king pawn two squares forward, or `g1f3` to develop the king knight instead. Castling is done simply by typing the movement of the king (e.g., `e1g1` for the white king.)


## Unit Testing
The unit test target builds and runs the Catch-based tests under `test/unit`.

```bash
make unit_test
```

For compact test output, first run `uv sync --dev`, then:

```bash
COMPACT=true make unit_test
```
