# philosophers

<p align="center">
  <img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/philosophersn.png" alt="Philosophers 42 project badge"/>
</p>

Philosophers is a 42 school project designed around Edsger Dijkstra's famous dining philosophers problem. For this project, we must learn about concurrent programming. For the project's mandatory part, we must find a solution to the problem using only threads and mutexes. For the bonus part, we must create a second program that uses processes and semaphores.

---

:us: Read my articles in English about this project:
* [Philosophers 01: Threads and Mutexes](https://www.codequoi.com/en/philosophers-01-threads-and-mutexes/)
* [Philosophers 02: Processes and Semaphores](https://www.codequoi.com/en/philosophers-02-processes-and-semaphores/)

:fr: Lire mes articles en français sur ce projet :
* [Philosophers 01 : threads et mutex](https://www.codequoi.com/philosophers-01-threads-et-mutex/)
* [Philosophers 02 : processus et sémaphores](https://www.codequoi.com/philosophers-02-processus-et-semaphores/)

---

## Status 

Finished 24/11/2022. Grade: 125%.

## General Description

The [subject](https://github.com/mcombeau/philosophers/blob/main/subject/philosophers.en.subject.pdf) describes the problem as follows:

* One or more philosophers sit at a round table.
* There is a large bowl of spaghetti in the middle of the table.
* The spaghetti can only be eaten with two forks.
* There are only as many forks as there are philosophers.
* Each philosopher successively eats, sleeps and thinks.
* If a philosopher hasn't eaten in a certain timeframe, he will die of starvation.
* Philosophers cannot communicate with each other.

In both the mandatory and bonus parts, we must create an algorithm that keeps the philosophers alive as far as possible. The programs must take several parameters:

* **```number_of_philosophers```**: the number of philosophers around the table,
* **```time_to_die```**: a number representing the time in milliseconds a philosopher has to live after a meal. If a philosopher hasn’t started eating time_to_die milliseconds after the beginning of his last meal or the beginning of the simulation, he will die.
* **```time_to_eat```**: a number representing the time in milliseconds a philosopher takes to finish his meal. During that time, the philosopher keeps his two forks in hand.
* **```time_to_sleep```**: the time in milliseconds that a philosopher spends sleeping.
* **```number_of_times_each_philosopher_must_eat```**: an optional argument that allows the program to stop if all the philosophers have eaten at least that many times. If this argument is not specified, the simulation carries on unless a philosopher dies.

The programs outputs a message each time a philosopher takes an action, which is formatted this way:

```
[timestamp_in_ms] [X] has taken a fork
[timestamp_in_ms] [X] is eating
[timestamp_in_ms] [X] is sleeping
[timestamp_in_ms] [X] is thinking
[timestamp_in_ms] [X] died
```

## Mandatory Part - Threads and Mutexes

In the mandatory part, each philosopher is a thread and each fork is protected by a mutex. There is exactly one fork between each philosopher and a philosopher must take the forks to his immediate left and right in order to eat.

<a href="https://www.codequoi.com/en/philosophers-01-threads-and-mutexes/"><img src="https://github.com/mcombeau/philosophers/blob/main/subject/philosophers.png" alt="Philosopher mandatory part illustration by mcombeau" style="width:500px;"/></a>

### Usage

Git clone the project and for the mandatory part:
```shell
cd philosophers/philo
```
To compile, use ```make``` or:

```shell
make BUILD=pretty
```

Run the program with the following arguments:

```shell
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philo_must_eat]
```

The arguments must be integers between 0 and INT_MAX representing a time in milliseconds. For example:

```shell
/philo 4 800 200 200 5
```

If the arguments are valid, the program will output the actions of each philosopher until one of them dies or until all of them have eaten number_of_times_each_philo_must_eat, if specified.

## Bonus Part - Processes and Semaphores

For the bonus part of this project, the problem is the same with a few differences: philosophers are now processes and the forks are now a counting semaphore. The forks are arranged at the center of the table when not in use and any philosopher can take any fork.

<a href="https://www.codequoi.com/en/philosophers-01-threads-and-mutexes/"><img src="https://github.com/mcombeau/philosophers/blob/main/subject/philosophers-bonus.png" alt="Philosopher bonus part illustration by mcombeau" style="width:500px;"/></a>

### Usage

Git clone the project and for the bonus part:
```shell
cd philosophers/philo_bonus
```
To compile, use ```make``` or:

```shell
make BUILD=pretty
```

Run the program with the following arguments:

```shell
./philo_bonus <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philo_must_eat]
```

Like the mandatory part, the arguments must be integers between 0 and INT_MAX representing a time in milliseconds. For example:

```shell
/philo_bonus 4 800 200 200 5
```

If the arguments are valid, the program will output the actions of each philosopher until one of them dies or until all of them have eaten number_of_times_each_philo_must_eat, if specified.

---
Made by mcombeau: mcombeau@student.42.fr | LinkedIn: [mcombeau](https://www.linkedin.com/in/mia-combeau-86653420b/) | Website: [codequoi.com](https://www.codequoi.com)
