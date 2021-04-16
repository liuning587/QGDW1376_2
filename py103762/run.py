"""run log file translate app"""
import sys
from master.app_trans import main


def run_analysis(file_path=''):
    """run master"""
    main(file_path)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        run_analysis(sys.argv[1])
    else:
        run_analysis()
