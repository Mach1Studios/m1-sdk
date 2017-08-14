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

There are six possibilities of choosing the rotation axes for Tait–Bryan angles. The six possible sequences are:

- x-y’-z″ (intrinsic rotations) or x-y-z (extrinsic rotations)
- y-z’-x″ (intrinsic rotations) or y-z-x (extrinsic rotations)
- z-x’-y″ (intrinsic rotations) or z-x-y (extrinsic rotations)
- x-z’-y″ (intrinsic rotations) or x-z-y (extrinsic rotations)
- z-y’-x″ (intrinsic rotations) or z-y-x (extrinsic rotations): the intrinsic rotations are known as: yaw, pitch and roll
- y-x’-z″ (intrinsic rotations) or y-x-z (extrinsic rotations)

#### Documentation on All Euler Angle Orders: 
The so-called "x-convention," illustrated above, is the most common definition. In this convention, the rotation given by Euler angles (phi,theta,psi), where

1. the first rotation is by an angle phi about the z-axis using D,

2. the second rotation is by an angle theta in [0,pi] about the former x-axis (now x^') using C, and

3. the third rotation is by an angle psi about the former z-axis (now z^') using B.

Note, however, that several notational conventions for the angles are in common use. Goldstein (1980, pp. 145-148) and Landau and Lifschitz (1976) use (phi,theta,psi), Tuma (1974) says (psi,theta,phi) is used in aeronautical engineering in the analysis of space vehicles (but claims that (phi,theta,psi) is used in the analysis of gyroscopic motion), while Bate et al. (1971) use  (Omega,i,omega). Goldstein remarks that continental authors usually use (psi,theta,phi), and warns that left-handed coordinate systems are also in occasional use (Osgood 1937, Margenau and Murphy 1956-64). Varshalovich (1988, pp. 21-23) uses the notation (alpha,beta,gamma) or (alpha^',beta^',gamma^') to denote the Euler angles, and gives three different angle conventions, none of which corresponds to the x-convention.