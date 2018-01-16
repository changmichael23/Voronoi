#pragma once
#include "Patch.hpp"

extern std::vector<Patch> patches;
extern Patch tmpPatch;
extern int nbPoints;
extern int pointIdx, patchIdx;
extern State gs;
extern EditMode em;

void StartNewPatch()
{
	gs = drawing;
	int n, m;

	std::cout << "Saisir le nombre de lignes du patch : ";
	std::cin >> n;
	std::cout << "Saisir le nombre de colonnes du patch : ";
	std::cin >> m;

	tmpPatch = Patch(n, m);
	nbPoints = n * m;
	pointIdx = 0;
}

void CancelPatch()
{
	if (patches.size() == 0)
	{
		gs = first;
	}
	else
	{
		gs = idle;
	}
}

void ConfirmPatch()
{
	gs = idle;
	patches.push_back(tmpPatch);
}

void RotatePatch(int i)
{
	gs = editing;
	patchIdx = 0;

	if (i == 0)
	{
		em = rotateX;
	}
	else if (i == 1)
	{
		em = rotateY;
	}
	else if (i == 2)
	{
		em = rotateZ;
	}
}

void TranslatePatch(int i)
{
	gs = editing;
	patchIdx = 0;

	if (i == 0)
	{
		em = translateX;
	}
	else if (i == 1)
	{
		em = translateY;
	}
	else if (i == 2)
	{
		em = translateZ;
	}
}

void ResizePatch()
{
	gs = editing;
	patchIdx = 0;
	em = resize;
}

void CancelEdit()
{
	gs = idle;
}

void ChangeRotationMethod()
{
	if (rm == matrix)
	{
		rm = quaternion;
	}
	if (rm == quaternion)
	{
		rm = matrix;
	}
}

void DeletePatch()
{
	gs = deleting;
	patchIdx = 0;
}

void QuitDelete()
{
	if (patches.size() == 0)
	{
		gs = first;
	}
	else
	{
		gs = idle;
	}
}

void ConfirmDelete()
{
	patches.erase(patches.begin() + patchIdx);
	if (patches.size() == 0)
	{
		QuitDelete();
	}
}