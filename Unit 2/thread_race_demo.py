import threading
import time

THREADS = 8
INCREMENTS = 2000
counter = 0


def unsafe_increment() -> None:
    """Increment shared counter without synchronization (race condition)."""
    global counter
    for _ in range(INCREMENTS):
        temp = counter
        # Force context switch opportunity between read and write.
        time.sleep(0.00001)
        counter = temp + 1


def safe_increment(lock: threading.Lock) -> None:
    """Increment shared counter with a lock (mutual exclusion)."""
    global counter
    for _ in range(INCREMENTS):
        with lock:
            temp = counter
            time.sleep(0.00001)
            counter = temp + 1


def run_unsafe_demo() -> int:
    global counter
    counter = 0
    workers = [threading.Thread(target=unsafe_increment) for _ in range(THREADS)]
    for worker in workers:
        worker.start()
    for worker in workers:
        worker.join()
    return counter


def run_safe_demo() -> int:
    global counter
    counter = 0
    lock = threading.Lock()
    workers = [threading.Thread(target=safe_increment, args=(lock,)) for _ in range(THREADS)]
    for worker in workers:
        worker.start()
    for worker in workers:
        worker.join()
    return counter


def main() -> None:
    expected = THREADS * INCREMENTS

    print("Unit 2 Verification: Race Condition vs Mutual Exclusion")
    print(f"Threads: {THREADS}, increments/thread: {INCREMENTS}")
    print(f"Expected final counter: {expected}\n")

    unsafe_value = run_unsafe_demo()
    print("1) Without lock (race condition)")
    print(f"   Observed counter: {unsafe_value}")
    print(f"   Lost updates: {expected - unsafe_value}\n")

    safe_value = run_safe_demo()
    print("2) With lock (mutual exclusion)")
    print(f"   Observed counter: {safe_value}")
    print(f"   Lost updates: {expected - safe_value}\n")

    if unsafe_value != expected and safe_value == expected:
        print("Result: Verified. Lock removes race-condition errors.")
    else:
        print("Result: Run again. Timing can vary by machine.")


if __name__ == "__main__":
    main()
