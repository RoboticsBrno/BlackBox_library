# Touchpad

## Wanted information
* Position of touch (angle in polar coordinates would suffice, distance from center would be nice to have).
* Applied pressure.
* Length of touch.

## Principle
Touchpad consists of two parallel circuit boards. One has four planar coils on it arranged with their centers forming vertices of a square. We measure their inductance, which changes based on the distance from the other circuit board.

## Current model
The current model is written for a rigid perfectly flat board with pivot at the center. However the actual model is a soft board affixed around the perimeter, which deforms based on where you put pressure on it.

### Problems - implementation
* The board deforms on its own and is not flat in the resting state.
* The board is possibly too soft (harder board has show somewhat better results)
* The only filtering present is floating average

### Problems - principle
* We cannot measure the range of values, which may come out of the sensor (The distance between the boars cannot be measured absolutely, because the boards may not always be perfectly assembled and their distance may vary between different instances)

### Problems - other
* Lack of annotated data, which tests could be run on