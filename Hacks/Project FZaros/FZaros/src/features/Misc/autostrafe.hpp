#pragma once
#include "../../sdk/vector.hpp"

#define M_PI		3.14159265358979323846

const double M_U_DEG = 360.0 / 65536;
const double M_U_RAD = M_PI / 32768;
#define PI_2 M_PI/2
#define PI_4 M_PI/4

class nigger
{
public:
	double anglemod_deg(double a)
	{
		return M_U_DEG * ((int)(a / M_U_DEG) & 0xffff);
	}

	double anglemod_rad(double a)
	{
		return M_U_RAD * ((int)(a / M_U_RAD) & 0xffff);
	}

	double point2line_distsq(const double pos[2],
		const double line_origin[2],
		const double line_dir[2])
	{
		double tmp[2] = { line_origin[0] - pos[0], line_origin[1] - pos[1] };
		double dotprod = line_dir[0] * tmp[0] + line_dir[1] * tmp[1];
		tmp[0] -= line_dir[0] * dotprod;
		tmp[1] -= line_dir[1] * dotprod;
		return tmp[0] * tmp[0] + tmp[1] * tmp[1];
	}

	double strafe_theta_opt(double speed, double L, double tauMA)
	{
		double tmp = L - tauMA;
		if (tmp <= 0)
			return PI_2;
		if (tmp < speed)
			return std::acos(tmp / speed);
		return 0;
	}

	void strafe_fme_vec(double vel[2], const double avec[2], double L,
		double tauMA)
	{
		double tmp = L - vel[0] * avec[0] - vel[1] * avec[1];
		if (tmp < 0)
			return;
		if (tauMA < tmp)
			tmp = tauMA;
		vel[0] += avec[0] * tmp;
		vel[1] += avec[1] * tmp;
	}

	void strafe_fric(double vel[2], double E, double ktau)
	{
		double speed = std::hypot(vel[0], vel[1]);
		if (speed >= E)
		{
			vel[0] *= 1 - ktau;
			vel[1] *= 1 - ktau;
			return;
		}

		double tmp = E * ktau;
		if (speed > tmp)
		{
			tmp /= speed;
			vel[0] -= tmp * vel[0];
			vel[1] -= tmp * vel[1];
			return;
		}

		vel[0] = 0;
		vel[1] = 0;
	}

	double strafe_fric_spd(double spd, double E, double ktau)
	{
		if (spd >= E)
			return spd * (1 - ktau);
		double tmp = E * ktau;
		if (spd > tmp)
			return spd - tmp;
		return 0;
	}

	void strafe_side(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double theta, double L, double tauMA, int dir)
	{
		double phi;
		// This is to reduce the overall shaking.
		if (theta >= PI_2 * 0.75)
		{
			Sdir = dir;
			Fdir = 0;
			phi = std::copysign(PI_2, dir);
		}
		else if (PI_2 * 0.25 <= theta && theta <= PI_2 * 0.75)
		{
			Sdir = dir;
			Fdir = 1;
			phi = std::copysign(PI_4, dir);
		}
		else
		{
			Sdir = 0;
			Fdir = 1;
			phi = 0;
		}

		if (std::fabs(vel[0]) > 0.1 || std::fabs(vel[1]) > 0.1)
			yaw = std::atan2(vel[1], vel[0]);
		yaw += phi - std::copysign(theta, dir);
		double yawcand[2] = {
			anglemod_rad(yaw), anglemod_rad(yaw + std::copysign(M_U_RAD, yaw))
		};
		double avec[2] = { std::cos(yawcand[0] - phi), std::sin(yawcand[0] - phi) };
		double tmpvel[2] = { vel[0], vel[1] };
		strafe_fme_vec(vel, avec, L, tauMA);
		avec[0] = std::cos(yawcand[1] - phi);
		avec[1] = std::sin(yawcand[1] - phi);
		strafe_fme_vec(tmpvel, avec, L, tauMA);

		if (tmpvel[0] * tmpvel[0] + tmpvel[1] * tmpvel[1] >
			vel[0] * vel[0] + vel[1] * vel[1])
		{
			vel[0] = tmpvel[0];
			vel[1] = tmpvel[1];
			yaw = yawcand[1];
		}
		else
			yaw = yawcand[0];
	}

	void strafe_line_opt(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double pos[2], double L, double tau, double MA,
		double line_origin[2], double line_dir[2])
	{
		double tauMA = tau * MA;
		double speed = std::hypot(vel[0], vel[1]);
		double theta = strafe_theta_opt(speed, L, tauMA);
		double ct = std::cos(theta);
		double tmp = L - speed * ct;
		if (tmp < 0)
		{
			strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, 1);
			return;
		}

		if (tauMA < tmp)
			tmp = tauMA;
		tmp /= speed;
		double st = std::sin(theta);
		double newpos_right[2], newpos_left[2];
		double avec[2];

		avec[0] = (vel[0] * ct + vel[1] * st) * tmp;
		avec[1] = (-vel[0] * st + vel[1] * ct) * tmp;
		newpos_right[0] = pos[0] + tau * (vel[0] + avec[0]);
		newpos_right[1] = pos[1] + tau * (vel[1] + avec[1]);

		avec[0] = (vel[0] * ct - vel[1] * st) * tmp;
		avec[1] = (vel[0] * st + vel[1] * ct) * tmp;
		newpos_left[0] = pos[0] + tau * (vel[0] + avec[0]);
		newpos_left[1] = pos[1] + tau * (vel[1] + avec[1]);

		bool rightgt = point2line_distsq(newpos_right, line_origin, line_dir) <
			point2line_distsq(newpos_left, line_origin, line_dir);
		strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, rightgt ? 1 : -1);
	}

	void MovementFix(CUserCmd* cmd, Vector3& realvec)
	{
		Vector3 vMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		Vector3 vMove2;
		g_Math.vectorAngles(vMove, vMove2);

		flYaw = DEG2RAD(cmd->viewAngles.y - realvec.y + vMove2.y);
		cmd->forwardmove = cos(flYaw) * flSpeed;
		cmd->sidemove = sin(flYaw) * flSpeed;

		if (90 < abs(cmd->viewAngles.x)
			) cmd->forwardmove *= -1;
	}

};