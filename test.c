t_vec rotate(t_vec axis, t_vec vec, int dir)
{
    t_vec           new_dir;
	const double    c = (1 - cos(dir * 0.1));
	const double    s = sin(dir * 0.1);

	new_dir.x = - vec.x * c * axis.y * axis.y 
	- vec.z * s * axis.y + c * vec.y * axis.x * axis.y 
	- vec.x * c * axis.z * axis.z + vec.y * s * axis.z
	+ c * vec.z * axis.x * axis.z + vec.x;
	new_dir.y = vec.y - c * vec.y * axis.x * axis.x 
	+ vec.z * s * axis.x + vec.x * c * axis.x * axis.y 
	- c * vec.y * axis.z * axis.z - vec.x * s * axis.z + c * vec.z * axis.y * axis.z;
	new_dir.z = vec.z - c * vec.z * axis.x * axis.x
	- vec.y * s * axis.x - c * vec.z * axis.y * axis.y + vec.x * s * axis.y 
	+ vec.x * c * axis.x * axis.z + c * vec.y * axis.y * axis.z;

	return (new_dir);
}