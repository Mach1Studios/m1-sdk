## Mach1 Internal Angle Standard

### Coordinate / Angle / Rotation Description Expectations:

* Rotations can be individually explained per axis with signed rotations
* Rotations are explained from a center perspective point of view (FPV - First Person View)

### Mach1 YPR Polar Expectation of Describing Orientation:

#### Common use: Mach1Decode API, Mach1DecodePositional API
* Yaw   (left -> right | where rotating left is negative)
* Pitch (down -> up | where rotating down is negative)
* Roll  (top-pointing-left -> top-pointing-right | where rotating top of object left is negative)

### Mach1 AED Expectation of Describing Polar Points:

#### Common use: Mach1Encode API
* Azimuth   (left -> right | where rotating left is negative)
* Elevation (down -> up | where rotating down is negative)
* Diverge   (backward -> forward | where behind origin of Azimuth/Elevation is negative)

### Mach1 XYZ Coordinate Expectation of Vector Points:
* X (left -> right | where -X is left)
* Y (front -> back | where -Y is back)
* Z (top -> bottom | where -Z is bottom)

#### Positional 3D Coords
* X+ = strafe right
* X- = strafe left
* Y+ = up
* Y- = down
* Z+ = forward
* Z- = backward

#### Orientation Euler
* Yaw[0]+ = rotate right [Range: 0->360 | -180->180]
* Yaw[0]- = rotate left [Range: 0->360 | -180->180]
* Pitch[1]+ = rotate up [Range: -90->90]
* Pitch[1]- = rotate down [Range: -90->90]
* Roll[2]+ = tilt right [Range: -90->90]
* Roll[2]- = tilt left [Range: -90->90]

### Angle Order Conventions List of Concerns:
1. Order of Yaw, Pitch, Roll (Defined as angle applied first, second and third).
2. Direction of transform around each pole's positive movement (left or right rotation).
3. integer Range before tranform completes 2(PI).

| Order        | airplane      | telescope | symbol | angular velocity |
| -------------:| -------------:| ---------:| ------:| ----------------:| 
| applied first | heading | azimuth   | θ (theta) |         yaw         |
| applied second      | attitude      | elevation | φ (phi) | pitch |
| applied last | bank | tilt    | ψ (psi) | roll |

### Euler Angle Orders:
- (yaw-pitch-roll)
- Unity: Left handed x-y-z (pitch-roll-yaw)
- Unreal Engine: Right handed z-y-x 
- (yaw-roll-pitch)