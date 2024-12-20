#### Authors:
    Jowi Aoun
    Param Desai
#### Description:
This project is designed for simulating multithreaded interactions between ghosts, hunters, and their environment in a haunted house scenario. The hunters carry a different type of equipment (EMF/TEMPERATURE/SOUND/FINGERPRINTS) and have to go around the house to catch 3 unique evidences in total to win. The ghost drops evidence based on its randomly selected type, of which drops 3 of these 4 types of evidence. For the hunters to lose and the ghost to win, the hunters have to either be too bored, or have a high enough fear level. At the end, all hunters that have left before reviewing successful evidence, will be printed to the output. This program includes many functionalities such as initializing and managing a house and ghost, hunters, logging activities, and utility functions.
Bonus points to validate:
- Use print formatting to make the simulation data clear and easy to read using consistent fixed-width columns
- Use Git and either GitLab or GitHub (using private repositories only) to version control your code (GitCommits.png)

#### Files:
- `main.c`:   The main entry point of the application.
              Contains Functions: main()
- `ghost.c`:  Handles the initialization and management of ghosts.
              Contains Functions: initGhost(), addGhost,   chooseGhostActions, moveGhostRooms, leaveEvidence(),  ghost_thread().
- `house.c`:  Manages house-related functionalities such as room      creation and connections.
              Contains Functions: initHouse(), initRoomList(), populateRooms(), createRoom(), connectRooms, addRoom(), cleanupHouse().
- `hunter.c`: Involves initialization and activities of hunters.
              Contains Functions: initHunter(), hunter_thread(), collectEvidence(), moveHunterRooms(), reviewEvidence().
- `logger.c`: Provides logging functionalities for various events and states.
              Contains Functions: l_hunterInit(), l_hunterMove(), l_hunterReview(), l_hunterCollect(), l_hunterExit(), l_ghostInit(), l_ghostMove(), l_ghostEvidence(), l_ghostExit().
- `utils.c`:  Contains utility functions like random number generation.
              Contains Functions: randInt(), randFloat(), randomGhost,
              evidenceToString(), ghostToString(), allocMemory(),  initEvidenceList(), addEvidence(), removeEvidence(),   removeHunter().
- `defs.h`:   Header file with definitions and function prototypes.
- `README.md`:This file. Contains description about the program.

#### Compilation: Compilation using Makefile (Option 1):
1. Navigate to the project directory.
2. Run the command `make` to compile the project.
3. This will generate the executable file called `final.exe`
4. Run the executable file
    ./final.exe
Note: to clean the object files and executables, run `make clean`

#### Compilation: Manual Compilation (Option 2):
1. Navigate to the project directory.
2. Compile each source file manually using the gcc compiler:
   gcc -c ghost.c
   gcc -c house.c
   gcc -c hunter.c
   gcc -c logger.c
   gcc -c main.c
   gcc -c utils.c
3. Link the object files to create the executable:
   gcc ghost.o house.o hunter.o logger.o main.o utils.o -o final.exe

#### Run:
- After compilation, run the program by executing the generated executable file:
  `./final.exe`

#### How to use:
1. Start the application by running the executable.
2. After following the compilation and run steps, the program will ask to provide hunter names for 4 hunters, provide and the program will begin running.
3. Once the program finished executing all the threads, the result of the run will be outputted to the console.

#### Generative AI:
No generative AI was used in this assignment.
