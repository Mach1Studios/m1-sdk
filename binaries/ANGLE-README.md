## Mach1 Internal Angle Standard

#### Positional 3D Coords
 X+ = strafe right
 X- = strafe left
 Y+ = up
 Y- = down
 Z+ = forward
 Z- = backward

 #### Orientation Euler
 Yaw[0]+ = rotate right [Range: 0->360 | -180->180]
 Yaw[0]- = rotate left [Range: 0->360 | -180->180]
 Pitch[1]+ = rotate up [Range: -90->90]
 Pitch[1]- = rotate down [Range: -90->90]
 Roll[2]+ = tilt right [Range: -90->90]
 Roll[2]- = tilt left [Range: -90->90]

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