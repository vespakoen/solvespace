const slvs = require('../../build/bin/slvslib')

slvs().then((lib) => {
    console.log(lib)

    //     Slvs_hGroup g;
    // double qw, qx, qy, qz;

    // g = 1;
    // /* First, we create our workplane. Its origin corresponds to the origin
    //  * of our base frame (x y z) = (0 0 0) */
    // sys.param[sys.params++] = Slvs_MakeParam(1, g, 0.0);
    // sys.param[sys.params++] = Slvs_MakeParam(2, g, 0.0);
    // sys.param[sys.params++] = Slvs_MakeParam(3, g, 0.0);
    // sys.entity[sys.entities++] = Slvs_MakePoint3d(101, g, 1, 2, 3);
})

