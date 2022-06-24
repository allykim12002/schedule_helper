

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool isValid(int row, int col, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched);

bool solveHelper(int row, int col, AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    // Rows = # of days
    // Columns = # of workers
    int numRows = avail.size();
    int numCols = dailyNeed;
    
    // Initialize the Schedule Array
    std::vector<Worker_T> schedRow;
    for (int i=0; i<numRows; i++) {
        // First make an array the size of the # of workers needed per day
        for (int j=0; j<numCols; j++) {
            schedRow.push_back(INVALID_ID);
        }
        // Add the vector created to each day
        sched.push_back(schedRow);
        schedRow.clear();
    }
    
    // Make a new available matrix to be able to modify it
    AvailabilityMatrix available = avail;
    // Call the solveHelper function at 0,0
    bool solved = solveHelper(0, 0, available, dailyNeed, maxShifts, sched);

    if (solved) {
        return true;
    }
    else {
        return false;
    }


}

bool isValid(int row, int col, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched) {
    // need = # of columns
    int need = dailyNeed;
    // days = # of rows
    int days = sched.size();

    // Get the worker ID that's currently in the sched array
    Worker_T worker = sched[row][col];
    
    size_t count = 0;

    // Go through the schedule to ensure that maxShifts is not violated
    for (int i=0; i < days; i++) {
        for (int j=0; j < need; j++) {
            if (sched[i][j] == worker) {
                count++;
            }
        }
    }
    if (count > maxShifts) {
        return false;
    }
    else {
        return true;
    }
}

bool solveHelper(int row, int col, AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched) {
    // days = # of rows 
    int days = avail.size();
    // need = # of columns
    int need = dailyNeed;
    // workers = # of all workers (columns of avail)
    Worker_T workers = avail[0].size();
    
    Worker_T scheduledWorker;
    if (sched[row][col] == INVALID_ID) {
        for (Worker_T i=0; i < workers; i++) {
            // If a worker is available on that day
            if (avail[row][i] == 1) {
                // Schedule the worker
                sched[row][col] = i;
                // Make worker unavailable
                avail[row][i] = 0;
                scheduledWorker = i;
                // Check validity of current schedule
                if (isValid(row, col, dailyNeed, maxShifts, sched) == true) {
                    // Check validity of next schedule
                    if (col < need-1) {
                        if (solveHelper(row, col+1, avail, dailyNeed, maxShifts, sched) == true) {
                            return true;
                        }
                    }
                    else if (row < days-1) {
                        if (solveHelper(row+1, 0, avail, dailyNeed, maxShifts, sched) == true) {
                            return true;
                        }
                    }
                    // Reached end of 2D array
                    else {
                        return true;
                    }
                }
            }
            // Reset the values to backtrack
            avail[row][scheduledWorker] = 1;
            sched[row][col] = INVALID_ID;
        }
    }
    return false;

}
