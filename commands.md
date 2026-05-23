Step 1 — Clone (with F´ submodule)
git clone --recursive <YOUR_REPO_URL> hello-project
cd hello-project
If you already cloned without submodules:

git submodule update --init --recursive
Why: lib/fprime/ is the F´ framework. Without it, nothing builds.

Step 2 — Python virtual environment
python3 -m venv fprime-venv
source fprime-venv/bin/activate
Why: Installs fprime-util, fprime-gds, and FPP tools in an isolated env. Run source fprime-venv/bin/activate every new terminal session.

Step 3 — Install Python tools
pip install --upgrade pip
pip install -r requirements.txt
Why: Pulls F´ tooling from requirements.txt → lib/fprime/requirements.txt.

Check:

which fprime-util fprime-gds
Step 4 — Generate CMake build
cd HiNamespace/FirstDeployment
fprime-util generate
Why: Creates build-fprime-automatic-native/ and registers HiComponent, HiEchoComponent, topology, and framework.

Step 5 — Build flight software
fprime-util build
Why: Autocodes .fpp → C++, compiles, installs to:

build-artifacts/Darwin/HiNamespace_FirstDeployment/bin/HiNamespace_FirstDeployment

Step 6 — Run GDS + app
fprime-gds
Why: Starts web GDS (usually http://127.0.0.1:5000) and the deployment binary over TCP.

Stop with Ctrl+C.

Step 7 — Test in GDS
Test	Component	Command	Expect
Local hello
hiCmpntInstance
SAY_HI → greeting Hello
SayHiEvent, GreetingCount ↑
Port-to-port
hiCmpntInstance
SEND_TO_PEER → code 42
On hiEchoInstance: MessageReceived, MessageCount ↑
Port path: SEND_TO_PEER → greetingOut → topology wire → hiEchoInstance.greetingIn.

Edit / rebuild loop
cd hello-project
source fprime-venv/bin/activate
cd HiNamespace/FirstDeployment
fprime-util build
# restart: fprime-gds
After new components or CMake changes: fprime-util generate then fprime-util build.

New component (tutorial wizard)
cd HiNamespace/Components
fprime-util new --component
Why: Interactive Cookiecutter wizard (8 questions). You still wire instances.fpp and topology.fpp by hand.

What’s in the repo
hello-project/
├── lib/fprime/              # F´ submodule
├── HiNamespace/
│   ├── Components/HiComponent/
│   ├── Components/HiEchoComponent/
│   ├── Ports/HiPorts.fpp
│   └── FirstDeployment/     # ← build & run here
├── requirements.txt
└── settings.ini
One-shot copy-paste (after clone)
cd hello-project
git submodule update --init --recursive
python3 -m venv fprime-venv
source fprime-venv/bin/activate
pip install --upgrade pip && pip install -r requirements.txt
cd HiNamespace/FirstDeployment
fprime-util generate
fprime-util build
fprime-gds