from typing import List
from sys import argv
import os
import subprocess
from glob import glob


class Grader:
    def __init__(self):
        self.score = 0
        self.thread = False
        self.mutex = False
        self.semaphore = False
        self.doctor = False
        self.laundry = False
        self.cwd = os.getcwd()
        self.create_output_dirs()

    def set_test(self, test: List[bool]):
        self.thread = test[0]
        self.mutex = test[1]
        self.semaphore = test[2]
        self.doctor = test[3]
        self.laundry = test[4]

    def create_output_dirs(self):
        dirs = ["thread", "mutex", "semaphore", "doctor", "laundry"]

        for dir in dirs:
            try:
                os.mkdir(f"{self.cwd}/test/{dir}/student_output/")
            except:
                pass

    def test_thread(self):
        test_scores = [10, 10, 10, 20]

        # run make in /test/thread
        try:
            subprocess.run("make thread", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        path_to_executables = f"{self.cwd}/test/thread/build/"
        path_to_student_output = f"{self.cwd}/test/thread/student_output/"
        executables = glob(path_to_executables + "*")

        for executable in executables:
            try:
                subprocess.run(
                    [
                        executable,
                    ],
                    stdout=open(
                        f"{self.cwd}/test/thread/student_output/{executable.split('/')[-1].split('.')[0]}.output",
                        "w",
                    ),
                    timeout=10,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")

        # compare each output file with the correct output file, stored in /test/thread/output
        # if correct, add score to total score
        correct_files = glob("test/thread/outputs/*")

        try:
            for file in correct_files:
                file_num = file.split(".")[0].split("/")[-1]
                correct = open(file, "r")
                student = open(path_to_student_output + file_num + ".output", "r")

                correct_lines = correct.readlines()
                student_lines = student.readlines()

                for index, line in enumerate(correct_lines):
                    if line != student_lines[index]:
                        print(f"Line {index} in file {file_num} does not match")
                        return

                correct.close()
                student.close()

                self.score += test_scores[int(file_num[-1]) - 1]
        except:
            print("Error running test_thread. Number of output files does not match")

    def test_mutex(self):
        test_scores = [2.5, 2.5, 2.5]

        path_to_executables = f"{self.cwd}/test/mutex/build/"
        path_to_student_output = f"{self.cwd}/test/mutex/student_output/"

        try:
            subprocess.run("make mutex", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        executables = glob(path_to_executables + "*")

        for executable in executables:
            try:
                subprocess.run(
                    [
                        executable,
                    ],
                    stdout=open(
                        f"{self.cwd}/test/mutex/student_output/{executable.split('/')[-1].split('.')[0]}.output",
                        "w",
                    ),
                    timeout=10,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")

        # compare each output file with the correct output file, stored in /test/mutex/output
        # if correct, add score to total score
        correct_files = glob("test/mutex/outputs/*")

        try:
            for file in correct_files:
                file_num = file.split(".")[0].split("/")[-1]
                correct = open(file, "r")
                student = open(path_to_student_output + file_num + ".output", "r")

                correct_lines = correct.readlines()
                student_lines = student.readlines()

                for index, line in enumerate(correct_lines):
                    if line != student_lines[index]:
                        print(f"Line {index} in file {file_num} does not match")
                        return

                correct.close()
                student.close()

                self.score += test_scores[int(file_num[-1]) - 1]
        except:
            print("Error running test_mutex. Number of output files does not match")

    def test_semaphore(self):
        test_scores = [2.5, 2.5, 2.5, 2.5, 2.5]

        # list all test files
        path_to_executables = f"{self.cwd}/test/semaphore/build/"
        path_to_student_output = f"{self.cwd}/test/semaphore/student_output/"

        try:
            subprocess.run("make semaphore", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        executables = glob(path_to_executables + "*")

        for executable in executables:
            try:
                subprocess.run(
                    [
                        executable,
                    ],
                    stdout=open(
                        f"{self.cwd}/test/semaphore/student_output/{executable.split('/')[-1].split('.')[0]}.output",
                        "w",
                    ),
                    timeout=10,
                )
            except Exception as e:
                print(f"Error running {executable}: {e}")

        # compare each output file with the correct output file, stored in /test/semaphore/output
        # if correct, add score to total score
        correct_files = glob("test/semaphore/outputs/*")
        try:
            for file in correct_files:
                file_num = file.split(".")[0].split("/")[-1]
                correct = open(file, "r")
                student = open(path_to_student_output + file_num + ".output", "r")

                correct_lines = correct.readlines()
                student_lines = student.readlines()

                for index, line in enumerate(correct_lines):
                    if line != student_lines[index]:
                        print(f"Line {index} in file {file_num} does not match")
                        return

                correct.close()
                student.close()

                self.score += test_scores[int(file_num[-1]) - 1]
        except:
            print("Error running test_semaphore. Number of output files does not match")

    def test_laundry(self):
        scores = [3] * 5

        path_to_executable = f"{self.cwd}/test/laundry/build/laundry.out"
        path_to_input = f"{self.cwd}/test/laundry/inputs/"
        path_to_correct_output = f"{self.cwd}/test/laundry/outputs/"
        path_to_student_output = f"{self.cwd}/test/laundry/student_output/"

        input_files = glob(path_to_input + "*")
        correct_output_files = glob(path_to_correct_output + "*")

        try:
            subprocess.run("make laundry", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        for file in input_files:
            file_num = file.split("/")[-1].split(".")[0]
            try:
                subprocess.run(
                    [
                        path_to_executable,
                    ],
                    stdin=open(file, "r"),
                    stdout=open(path_to_student_output + file_num + ".out", "w"),
                    timeout=10,
                )
            except Exception as e:
                print(f"Error running {file}: {e}")

        try:
            for file in correct_output_files:
                file_num = file.split("/")[-1].split(".")[0]
                correct = open(file, "r")
                student = open(path_to_student_output + file_num + ".out", "r")

                correct_lines = correct.readlines()
                student_lines = student.readlines()

                for index, line in enumerate(correct_lines):
                    if line != student_lines[index]:
                        return

                correct.close()
                student.close()

                self.score += scores[int(file_num) - 1]
        except:
            print("Error running test_laundry. Number of output files does not match")

    def test_doctor(self):
        scores = [3] * 5

        path_to_executable = f"{self.cwd}/test/doctor/build/doctor.out"
        path_to_input = f"{self.cwd}/test/doctor/inputs/"
        path_to_correct_output = f"{self.cwd}/test/doctor/outputs/"
        path_to_student_output = f"{self.cwd}/test/doctor/student_output/"

        input_files = glob(path_to_input + "*")
        correct_output_files = glob(path_to_correct_output + "*")

        try:
            subprocess.run("make doctor", cwd=f"{self.cwd}", shell=True, check=True)
        except Exception as e:
            print(f"Error running make {e}")
            return

        for file in input_files:
            file_num = file.split("/")[-1].split(".")[0]
            try:
                subprocess.run(
                    args=f"{path_to_executable} | tee {path_to_student_output + file_num + '.out'}",
                    stdin=open(file, "r"),
                    # stdout=open(path_to_student_output + file_num + ".out", "w"),
                    timeout=10,
                    shell=True,
                )
            except Exception as e:
                print(f"Error running {file}: {e}")

        try:
            for file in correct_output_files:
                file_num = file.split("/")[-1].split(".")[0]
                correct = open(file, "r")
                student = open(path_to_student_output + file_num + ".out", "r")

                correct_lines = correct.readlines()
                student_lines = student.readlines()

                for index, line in enumerate(correct_lines):
                    if line != student_lines[index]:
                        print(f"Line {index} in file {file_num} does not match")
                        return

                correct.close()
                student.close()

                self.score += scores[int(file_num) - 1]
        except:
            print("Error running test_doctor. Number of output files does not match")

    def run(self):
        if self.thread:
            print("Testing Threads")
            self.test_thread()
            print(f"Score: {self.score}\n")
        if self.mutex:
            print("Testing Mutex")
            self.test_mutex()
            print(f"Score: {self.score}\n")

        if self.semaphore:
            print("Testing Semaphore")
            self.test_semaphore()
            print(f"Score: {self.score}\n")

        if self.doctor:
            print("Testing Doctor")
            self.test_doctor()
            print(f"Score: {self.score}\n")

        if self.laundry:
            print("Testing Laundry")
            self.test_laundry()
            print(f"Score: {self.score}\n")

        print(f"Testing Completed. Final Score: {self.score}")


def main():
    # Initialize a list of bools, all set to false
    flags = [False] * 5

    # Check for command line arguments
    for arg in argv[1:]:
        if arg == "-t":
            flags[0] = True
        elif arg == "-m":
            flags[1] = True
        elif arg == "-s":
            flags[2] = True
        elif arg == "-d":
            flags[3] = True
        elif arg == "-l":
            flags[4] = True

    if len(argv) == 1:
        flags = [True] * 5

    # Initialize a grader object
    grader = Grader()
    grader.set_test(flags)
    print("Running tests...")
    grader.run()


if __name__ == "__main__":
    main()
