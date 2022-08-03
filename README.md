# philosophers

Philosophers is a 42 school project designed to learn about threads, mutexes and semaphores.

---

:us: Read my article in English about this project: [Philosophers 01: Threads and Mutexes](https://www.codequoi.com/en/philosophers-01-threads-and-mutexes/)

:fr: Lire mon article en français sur ce projet :  [Philosophers 01 : threads et mutex](https://www.codequoi.com/philosophers-01-threads-et-mutex/)

---

## Status 

Work in progress.

## Usage

Git clone the project and cd into its subfolder, philo. Then use ```make``` to compile.

Run the program with the following arguments:

```shell
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philo_must_eat]
```

The arguments must be integers between 0 and INT_MAX representing a time in milliseconds.

If the arguments are valid, the program will output the actions of each philosopher until one of them dies or until all of them have eaten number_of_times_each_philo_must_eat, if specified.

---
Made by mcombeau: mcombeau@student.42.fr | LinkedIn: [mcombeau](https://www.linkedin.com/in/mia-combeau-86653420b/) | Website: [codequoi.com](https://www.codequoi.com)
