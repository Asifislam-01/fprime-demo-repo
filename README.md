# HiNamespace F´ Project (hello-project)

[F Prime (F´)](https://fprime.jpl.nasa.gov/) flight-software project with a **Hello World** component, a **port-to-port** demo between two components, and a full **FirstDeployment** topology (command dispatch, events, telemetry, TCP comms).

Use this guide to clone, build, run, and extend the project on a new machine.

---

## What is in this project?

| Path | Purpose |
|------|---------|
| `lib/fprime/` | F´ framework (git submodule) |
| `HiNamespace/Components/HiComponent/` | Active component: `SAY_HI`, `SEND_TO_PEER`, `greetingOut` port |
| `HiNamespace/Components/HiEchoComponent/` | Receiver: `greetingIn` port, events, telemetry |
| `HiNamespace/Ports/HiPorts.fpp` | Custom `HiPing` port type (`code: U32`) |
| `HiNamespace/FirstDeployment/` | Runnable deployment + GDS config |
| `settings.ini` | Project root and framework path for `fprime-util` |

**Port-to-port wiring** (in `HiNamespace/FirstDeployment/Top/topology.fpp`):

```
hiCmpntInstance.greetingOut -> hiEchoInstance.greetingIn
```

---

## System requirements

1. **macOS**, **Linux**, or **Windows with WSL2**
2. **git** (with submodule support)
3. **Python 3.9+** and **pip**
4. **C++ compiler**: Clang or GCC (`clang++` / `g++`)

Quick checks:

```bash
git --version
python3 --version
clang++ --version   # or: g++ --version
```

Official F´ install notes: [Installing F´](https://fprime.jpl.nasa.gov/latest/docs/getting-started/installing-fprime/)

---

## 1. Clone the repository

Clone **with submodules** so `lib/fprime` is populated:

```bash
git clone --recursive <YOUR_REPO_URL> hello-project
cd hello-project
```

If you already cloned without `--recursive`:

```bash
cd hello-project
git submodule update --init --recursive
```

**What this does:** Downloads this project and the pinned F´ framework under `lib/fprime/`. Without the submodule, builds will fail.

---

## 2. Create and activate the Python virtual environment

One virtual environment per project (recommended by NASA F´ docs).

```bash
cd hello-project
python3 -m venv fprime-venv
source fprime-venv/bin/activate
```

On Windows (PowerShell):

```powershell
python -m venv fprime-venv
.\fprime-venv\Scripts\Activate.ps1
```

**What this does:** Isolates F´ Python tools (`fprime-util`, `fprime-gds`, FPP autocoders) from your system Python.

You should see `(fprime-venv)` in your shell prompt. **Activate this venv every time** you work on the project.

---

## 3. Install F´ Python dependencies

```bash
pip install --upgrade pip
pip install -r requirements.txt
```

**What this does:** Installs tools from `requirements.txt`, which pulls in `lib/fprime/requirements.txt` (FPP, GDS, CMake helpers, etc.).

Verify tools are available:

```bash
which fprime-util fprime-gds
fprime-util --help
```

If you see `command not found`, the venv is not activated.

---

## 4. Generate the build system

All build commands below are run from the **deployment** directory:

```bash
cd HiNamespace/FirstDeployment
fprime-util generate
```

**What this does:**

- Configures CMake under `build-fprime-automatic-native/` at the project root
- Registers components, ports, topology, and F´ framework modules
- Prepares autocoder output directories

Run `generate` again when you add a new deployment, change CMake structure, or the build directory is missing.

---

## 5. Build the flight software

```bash
fprime-util build
```

**What this does:**

- Runs FPP autocoders (`.fpp` → generated C++ headers/sources)
- Compiles the deployment binary and libraries
- Installs artifacts to:

  ```
  build-artifacts/<platform>/HiNamespace_FirstDeployment/
  ```

  On macOS, `<platform>` is usually `Darwin`.

Typical outputs:

| Artifact | Location |
|----------|----------|
| Runnable binary | `build-artifacts/Darwin/HiNamespace_FirstDeployment/bin/HiNamespace_FirstDeployment` |
| GDS dictionary | `build-artifacts/Darwin/HiNamespace_FirstDeployment/dict/FirstDeploymentTopologyDictionary.json` |

Fix any compile errors before continuing. A successful build ends without `FAILED` / `ERROR` in the log.

---

## 6. Run the Ground Data System (GDS) and application

From `HiNamespace/FirstDeployment` (venv still active):

```bash
fprime-gds
```

**What this does:**

- Starts the **F´ GDS** web UI (default: http://127.0.0.1:5000)
- Launches `HiNamespace_FirstDeployment` and connects over TCP on localhost
- Loads the topology dictionary so you can send commands and view events/telemetry

Stop with `Ctrl+C` in the terminal.

### Run GDS and app separately (optional)

**Terminal 1 — GDS only:**

```bash
cd hello-project
source fprime-venv/bin/activate
cd HiNamespace/FirstDeployment
fprime-gds --no-app
```

**Terminal 2 — application only:**

```bash
cd hello-project/build-artifacts/Darwin/HiNamespace_FirstDeployment/bin
./HiNamespace_FirstDeployment -a 127.0.0.1 -p 50000
```

---

## 7. Test in the GDS UI

Open the GDS URL in a browser (shown in the terminal, usually port **5000**).

### Test A — HiComponent only (`SAY_HI`)

1. Open the **Commanding** tab.
2. Select component **`hiCmpntInstance`**.
3. Send command **`SAY_HI`** with `greeting` = e.g. `Hello`.
4. Check **Events** for `SayHiEvent` on `hiCmpntInstance`.
5. Check **Channels** for telemetry **`GreetingCount`** incrementing.

**Flow:** GDS → Command Dispatcher → `HiComponent` handler → event + telemetry (no port involved).

### Test B — Port-to-port (`SEND_TO_PEER` → HiEcho)

1. **Commanding** → **`hiCmpntInstance`**.
2. Send **`SEND_TO_PEER`** with `code` = e.g. `42`.
3. Check **Events** on **`hiEchoInstance`**: `MessageReceived` — “HiEcho received ping code 42”.
4. Check **Channels** on **`hiEchoInstance`**: **`MessageCount`** increments.

**Flow:**

```
GDS → CmdDisp → hiCmpntInstance.SEND_TO_PEER
             → greetingOut_out(0, code)      [output port]
             → topology connection         [HiPortTest]
             → hiEchoInstance.greetingIn   [async input port]
             → greetingIn_handler          [HiEchoComponent.cpp]
             → event + telemetry on hiEchoInstance
```

---

## 8. Day-to-day development loop

After editing `.fpp`, `.cpp`, or `.hpp` files:

```bash
cd hello-project
source fprime-venv/bin/activate
cd HiNamespace/FirstDeployment
fprime-util build
```

Restart GDS (`Ctrl+C`, then `fprime-gds`) to run the new binary.

If you add a **new component** or change **CMakeLists.txt**:

```bash
fprime-util generate
fprime-util build
```

### Create a new component (interactive wizard)

From the project’s `Components` directory:

```bash
cd hello-project
source fprime-venv/bin/activate
cd HiNamespace/Components
fprime-util new --component
```

Answer the prompts (name, active/passive, commands, events, telemetry, etc.). The wizard does **not** wire topology connections — add instances in `FirstDeployment/Top/instances.fpp` and connections in `topology.fpp` yourself.

---

## Project layout (reference)

```
hello-project/
├── fprime-venv/              # Python venv (created locally, not in git)
├── lib/fprime/               # F´ framework submodule
├── HiNamespace/
│   ├── Ports/HiPorts.fpp
│   ├── Components/
│   │   ├── HiComponent/
│   │   └── HiEchoComponent/
│   └── FirstDeployment/      # Deployment: build & run from here
│       ├── Main.cpp
│       └── Top/
│           ├── topology.fpp
│           └── instances.fpp
├── build-fprime-automatic-native/   # CMake/ninja (generated)
├── build-artifacts/                 # Binaries & dictionary (generated)
├── requirements.txt
└── settings.ini
```

Generated folders (`build-*`, `fprime-venv`, `logs/`) are listed in `.gitignore`.

---

## Troubleshooting

| Problem | What to try |
|---------|-------------|
| `fprime-util: command not found` | `source fprime-venv/bin/activate` |
| Empty `lib/fprime` | `git submodule update --init --recursive` |
| Build fails after FPP edits | `fprime-util generate` then `fprime-util build` |
| GDS shows old commands | Rebuild, restart `fprime-gds` |
| No events in GDS | Confirm app is running; check `HiNamespace/FirstDeployment/logs/` |
| `fprime-util new` asks no questions | Run in a real interactive terminal, from `HiNamespace/Components/`, not inside another component folder |

More help: [F´ troubleshooting](https://fprime.jpl.nasa.gov/latest/docs/getting-started/installing-fprime/#troubleshooting)

---

## Learn more

- [F´ Hello World Tutorial](https://fprime.jpl.nasa.gov/latest/tutorials-hello-world/docs/hello-world/)
- [F´ User Manual](https://fprime.jpl.nasa.gov/latest/docs/user-manual/)
- [F´ Website](https://fprime.jpl.nasa.gov/)

---

## License

This project uses the F´ framework; see `lib/fprime/LICENSE.txt` for framework licensing.
