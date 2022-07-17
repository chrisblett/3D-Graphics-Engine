#ifndef MODEL_H
#define MODEL_H

class Mesh;

class Model
{
protected:
	Mesh* m_pMesh;
public:
	Model(Mesh* pMesh);

	const Mesh& GetMesh() { return *m_pMesh; }

	virtual void Render();	
};

#endif
