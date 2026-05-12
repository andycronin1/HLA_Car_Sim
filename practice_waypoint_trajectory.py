from dataclasses import dataclass
from typing import List


@dataclass
class Waypoint:
    position: List[float]  # [x, y, z]
    timestamp: float       # seconds
    cumulative_time: float = 0.0


@dataclass
class Trajectory:
    waypoints: List[Waypoint]


def position_at_time(trajectory: Trajectory, t: float) -> List[float]:
    """
    Practice task:
    Return the [x, y, z] position of an object on the trajectory at time t.

    Rules for this exercise:
    1) Assume waypoints are sorted by timestamp in ascending order.
    2) If t is before the first waypoint timestamp, return the first waypoint position.
    3) If t is after the last waypoint timestamp, return the last waypoint position.
    4) If t falls between two waypoints, linearly interpolate each coordinate.
    5) If t exactly equals a waypoint timestamp, return that waypoint position.

    You should implement this function.
    """
    waypoints = trajectory.waypoints

    if not waypoints:
        raise ValueError("trajectory must contain at least one waypoint")

    first_waypoint = waypoints[0]
    last_waypoint = waypoints[-1]

    if t <= first_waypoint.timestamp:
        return list(first_waypoint.position)

    if t >= last_waypoint.timestamp:
        return list(last_waypoint.position)

    for index in range(1, len(waypoints)):
        start_waypoint = waypoints[index - 1]
        end_waypoint = waypoints[index]

        if t == end_waypoint.timestamp:
            return list(end_waypoint.position)

        if t < end_waypoint.timestamp:
            segment_duration = end_waypoint.timestamp - start_waypoint.timestamp
            if segment_duration <= 0:
                raise ValueError("waypoint timestamps must be strictly increasing")

            ratio = (t - start_waypoint.timestamp) / segment_duration
            return [
                start_coordinate + ratio * (end_coordinate - start_coordinate)
                for start_coordinate, end_coordinate in zip(
                    start_waypoint.position, end_waypoint.position
                )
            ]

    return list(last_waypoint.position)


# Four-waypoint practice data set.
PRACTICE_TRAJECTORY = Trajectory(
    waypoints=[
        Waypoint([0.0, 0.0, 0.0], 0.0),
        Waypoint([10.0, 0.0, 0.0], 10.0),
        Waypoint([10.0, 10.0, 0.0], 20.0),
        Waypoint([20.0, 10.0, 10.0], 30.0),
    ]
)


# Expected checks (do not implement here; use these to test your solution):
# t = -5.0  -> [0.0, 0.0, 0.0]
# t = 0.0   -> [0.0, 0.0, 0.0]
# t = 5.0   -> [5.0, 0.0, 0.0]
# t = 10.0  -> [10.0, 0.0, 0.0]
# t = 15.0  -> [10.0, 5.0, 0.0]
# t = 25.0  -> [15.0, 10.0, 5.0]
# t = 30.0  -> [20.0, 10.0, 10.0]
# t = 100.0 -> [20.0, 10.0, 10.0]


if __name__ == "__main__":
    print("Practice question scaffold created.")
    print("Implement position_at_time, then test with the expected checks in comments.")


