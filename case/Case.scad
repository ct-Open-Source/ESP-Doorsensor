module sensorCase_lower() {
    difference() {
        union() {
            translate([0, 0, 7/2]) cube([87+8+4, 26+4, 7], center=true);
            translate([0, 0, 8/2]) cube([87+8, 26, 8], center=true);
        }
        translate([-42, 0, 19/2+2]) rotate([0, 90, 0]) cylinder(d=19, h=84, $fn=32);
        for(i=[-1, 1]) for(j=[-1, 1]) {
            translate([i*(87/2+2), j*10, -1]) {
                cylinder(d=3.2, h=50, $fn=32);
                cylinder(d=6.5, h=3.2, $fn=6);
            }
        }
    }
}

module sensorCase_upper() {
    difference() {
        translate([0, 0, (27+2-5)/2]) cube([87+8+4, 26+4, 27+2-5], center=true);
        translate([-87/2-4, -26/2, 2+3.5]) cube([87+8, 26, 27+2-5]);
        translate([-58/2+(87+8-58)/2, -26/2, 2]) cube([58, 26, 27+2-5]);
        for(i=[-1, 1]) for(j=[-1, 1]) {
            translate([i*(87/2+2), j*10, -1]) cylinder(d=3.2, h=50, $fn=32);
        }
        translate([87/2, -8, -0.5]) cube([10, 13, 8.5]);
        hull() {
            for(i=[0, 1]) translate([-87/2-3, 0, 20+i*10]) rotate([0, -90, 0]) cylinder(d=1.6, h=10, $fn=16);
        }
    }
}

sensorCase_lower();
//color("blue") translate([0, 0, 24]) rotate([180, 0, 0]) sensorCase_upper();
translate([0, 32, 0]) sensorCase_upper();