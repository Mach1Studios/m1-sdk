# Common Issues
The following is a list of common heard issues during implementation and include audio tools to help find these issues as well as basic descriptions of their behavior and how they can be avoided.

### Orientation Latency Issues
<aside class="notice">Orientation Latency is usually the biggest concern in the current climate of devices that need some sort of headtracking or window tracking. This is an issue for visuals but just as much for audio as well, if the angles inputted into the Mach1Decode functions are late then so are the returned coefficients which leads to the user losing immersion and effect.
</aside>

### Orientation Rate Issues (Zipper)
<aside class="notice">
If the rate of angles sending to the app layer with Mach1Decode integrated is too slow, this can cause some audible distortion / clicking issues added to the mix. The effect will sound like a zipper as you headtrack faster and faster. This is due to the difference of the returned coefficients being too large from result to result, causing the gain change applied to be too large and creating a "jump". We have included ways to filter the input angles into our functions however this is a band-aid to the issue if your target device has this issue. The idea is to filter and delay the inputting angles adding more intermidiary angles between each "jump" but at the cost of Orientation Latency (see above).
</aside>

### Audio/Visual Sync Issues
<aside class="notice">
It is important to ensure sync between a visual element in your project / application and the spatial audio mix. Ensure they are managed by the same function receiving the same call.
</aside>

### Spatial Decoding Phase Issues
<aside class="notice">
If you have your spatial audio streams coming from different audio managers that do not have sync between them you will start hearing strange channel phasing issues while you track your head orientation or device orientation. Eliminate these by ensuring your audio is called at the same time and managed properly.
</aside>
