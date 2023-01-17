#include "particleSystem.h"
#include "gaussianParticleGenerator.h"
#include "uniformParticleGenerator.h"

GameManager::GameManager(physx::PxScene* Scene, physx::PxPhysics* Physics)
{
	scene = Scene;
	physics = Physics;
	pFR = new ParticleForceRegistry();
	gravGen = new GravityForceGenerator({ 0,-9.8,0 });
	windGen = new WindOfChangeForceGenerator({ 20,0,0 }, 0.55, 0);
	//windGen->isOn = true;
	whirl = new WhirlwindOfChangeForceGenerator(7, { 50,50,-50 }, { 6,2,0 });
	//whirl->isOn = true;

	boom = new ExplosionBoomForceGenerator(200, 5, { 0,0,0 }, 2);


	createLevels();

	//renderLifes();

	//boom->isOn = true;
}

void GameManager::addParticleGen(/*GeneratorName gn*/)
{
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new GaussianParticleGenerator("Gaussian", { 50, 50, -50 }, { -16,150,-27 }, { 15,1,15 }, { 1,1,1 }, 1, GAUSSIAN_BALL)));
	_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new UniformParticleGenerator({ 0, 0, 0 }, { 0, 0, -100 })));
	//_particle_generators.push_back(std::shared_ptr<ParticleGenerator>(new UniformParticleGenerator("Uniform", { 0, 50, 0 }, { 0,10,27 }, 12, GAUSSIAN_BALL)));
	/*switch (gn)
	{
	case GAUSSIAN:

		break;
	case UNIFORM:
		break;
	case FIREWORK:
		_firework = new Firework({ 0,50,0 }, { 0,10,27 });
		break;
	default:
		break;
	}*/
}

void GameManager::update(double t)
{
	//cout << GetCamera()->getDir().x <<" "<< GetCamera()->getDir().y << " " << GetCamera()->getDir().z << endl;
	//renderLifes();
	//pLife->setPosition(GetCamera()->getDir());
	pFR->updateForce(t);
	//act generadores
	for (auto gen : _particle_generators) {
		if (gen->isOperative()) {
			std::list<Particle*> lista = gen->generateParticles();
			for (auto partic : lista) {
				_particles.push_back(partic);

			}
		}
	}
	//act particulas
	auto it = _particles.begin();
	while (it != _particles.end()) {
		if ((*it)->particleExists()) {
			(*it)->update(t);
			if (gravGen->isOn) pFR->addRegistry(gravGen, *it);
			if (windGen->isOn) pFR->addRegistry(windGen, *it);
			if (whirl->isOn) pFR->addRegistry(whirl, *it);


			++it;
		}
		else {
			pFR->deleteParticleRegistry(*it);
			onParticleDeath(*it);

			delete* it;
			*it = nullptr;
			it = _particles.erase(it);
		}
	}
	//turn of whirl 
	if (whirlOn) {
		whirlTimer -= t;
		if (whirlTimer <= 0) {
			whirlOn = false;
			stopLoseFeedback();
		}
	}
	/*if (lastShoot + 5000 <= t) {*/
	if (ActualLifes <= 0 && !hasLost) {
		deleteAllFruits();
		ActualLifes = 0;
		points = 0;
		glClearColor(1.0, 0.0, 0.0, 0.0);
		getParticleGenerator("Gaussian")->changeOperative();
		whirl->changeWhirl();
		isPlayerOnLevel = false;
		hasLost = true;
		whirlOn = true;
		//shootFirework(0);
	}
	lastShoot -= t;
	if (isPlayerOnLevel && lastShoot <= 0 && actualLevel->fruitsGeneratedInLevel < actualLevel->numOfFruits) {
		addFruit(t);
		lastShoot = actualLevel->frecuency;
	}
	if (isPlayerOnLevel && actualLevel->fruitsGeneratedInLevel == actualLevel->numOfFruits && !hasWon && !hasLost && fruits.empty()) {
		isPlayerOnLevel = false;
		p3->kill();
		victoryFireworks();
		hasWon = true;
	}
	/*}*/
	collisionsBetweenFruitsAndFloor(t);
	collisionsBetweenFruitsAndWeapons(t);

}

std::shared_ptr<ParticleGenerator> GameManager::getParticleGenerator(std::string gn)
{
	for (auto e : _particle_generators)
	{
		if ((*e).getGeneratorName() == gn) return e;
	}
	return nullptr;
}

void GameManager::onParticleDeath(Particle* p)
{
	Firework* fk = dynamic_cast<Firework*>(p);

	if (fk != nullptr) {
		auto part = fk->explode();
		for (auto p : part)
			_particles.push_back(p);
		//part.clear();
	}
}

void GameManager::generateFireworkSystem()
{
	//std::shared_ptr<ParticleGenerator>gF(new GaussianParticleGenerator("Firework", { 0, 0, 0 }, { 0,20,0 }, { 0,0,0 }, { 10,10,10 }, 4, FIREWORKS));
	std::shared_ptr<ParticleGenerator>gF(new CircleGenerator(30, 10));
	auto f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 30, 0));
	f->addGenerator(gF); f->numDivisions = 0;
	f->setTime(1.5);
	_firework_pool.push_back(f);
	gF->changeOperative();
	_particle_generators.push_back(gF);
	//
	//std::shared_ptr<ParticleGenerator>gF(new GaussianParticleGenerator("Firework", { 0, 0, 0 }, { 0,20,0 }, { 0,0,0 }, { 10,10,10 }, 4, FIREWORKS));
	/*f = new Firework(Vector3(1000000000, 0, 0), Vector3(0, 60, 0));
	f->addGenerator(gF);
	f->setTime(1.5);
	_firework_pool.push_back(f);
	gF->changeOperative();
	_particle_generators.push_back(gF);*/

}

bool GameManager::isFireworkAlive()
{
	return(_firework != nullptr);
}
void GameManager::shootFirework(int type)
{
	auto r = rand() % ((50 - 20) + 1) + 20;
	auto r2 = rand() % ((-50 - -80) + 1) + -80;

	auto f = _firework_pool[type]->clone();
	f->setPosition({ (float)r,50,(float)r2 });
	_particles.push_back(f);
}
void GameManager::generateSpringDemo()
{
	////Particle* p1=new Particle()
	//Particle* p1 = new Particle(MUELLE, { -10,10,0 }, { 0,0,0 });
	//Particle* p2 = new Particle(MUELLE, { 10,10,0 }, { 0,0,0 });

	//auto f1 = new SpringForceGenerator(10, 10, p2);

	//auto f2 = new SpringForceGenerator(10, 10, p1);

	//pFR->addRegistry(f1, p1);
	//pFR->addRegistry(f2, p2);

	//_particles.push_back(p1);
	//_particles.push_back(p2);


	 //p3 = new Particle(MUELLE, { 0,30,-50 }, { 0,0,0 });


	f3 = new AnchoredSpringFG(20, 10, { 0.0,50.0,-50.0 });

	/*pFR->addRegistry(f3, p3);
	_particles.push_back(p3);*/
}

void GameManager::slinky()
{
	float y = 60.0f;
	//Particle* pAnt;

	Particle* p = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p);
	y -= 5;
	Particle* p2 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p2);
	pFR->addRegistry(gravGen, p2);
	y -= 5;
	Particle* p3 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p3);
	pFR->addRegistry(gravGen, p3);
	y -= 5;
	Particle* p4 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p4);
	pFR->addRegistry(gravGen, p4);
	y -= 5;
	Particle* p5 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p5);
	pFR->addRegistry(gravGen, p5);
	y -= 5;
	Particle* p6 = new Particle(MUELLE, { 0,y,0 }, { 0,0,0 });
	_particles.push_back(p6);
	pFR->addRegistry(gravGen, p6);

	auto f = new SpringForceGenerator(10, 1, p);
	pFR->addRegistry(f, p2);
	auto f1 = new SpringForceGenerator(10, 1, p2);
	pFR->addRegistry(f1, p3);
	auto f2 = new SpringForceGenerator(20, 1, p3);
	pFR->addRegistry(f2, p4);
	auto f3 = new SpringForceGenerator(30, 1, p4);
	pFR->addRegistry(f3, p5);
	auto f4 = new SpringForceGenerator(40, 1, p5);
	pFR->addRegistry(f3, p6);

}

void GameManager::buoyancy()
{
	Particle* p = new Particle(BOX_PART, { 0,21,0 }, { 0,0,0 });
	p->setMass(50.0);
	_particles.push_back(p);

	auto f = new BuoyancyForceGenerator(12, 7, 1000);
	pFR->addRegistry(gravGen, p);

	pFR->addRegistry(f, p);
}
//_________
void GameManager::createScene() {
	floor = physics->createRigidStatic(PxTransform({ 50,50,-50 }));
	PxShape* shape = CreateShape(PxBoxGeometry(30, 0.1, 30));
	floor->attachShape(*shape);
	floor->setName("Floor");
	floorItem = new RenderItem(shape, floor, { 0.9,0.1,0.65,1 });
	scene->addActor(*floor);

	//StaticBody* s = new StaticBody();
	//s->body = floor;/* s->_remaining_time = 5*/; s->rendIt = item; 
	//PxRigidStatic* wall = physics->createRigidStatic(PxTransform({ 10,10,-30 }));
	////PxRigidDynamic* wall = gPhysics->createRigidDynamic(PxTransform({ 10,30,-30}));
	//PxShape* shapeWall = CreateShape(PxBoxGeometry(40, 20, 5));
	//wall->attachShape(*shapeWall);
	//wall->setName("Floor");
	//auto item = new RenderItem(shapeWall, wall, { 0.4,0.3,0.7,1 });
	//scene->addActor(*wall);

}
void GameManager::createLevels() {
	//level1
	level1 = new Level();
	level1->numOfFruits = 1;
	level1->frecuency = 100;
	//level2
	level2 = new Level();
	level2->numOfFruits = 10;
	level2->frecuency = 250;
	//level3
	level3 = new Level();
	level3->numOfFruits = 25;
	level3->frecuency = 100;

	//actualLevel = level1;
}
void GameManager::startNextLevel() {
	if (isPlayerOnLevel) return;
	//if no level has been started
	if (actualLevel == nullptr) {
		actualLevel = level1;
		isPlayerOnLevel = true;
	}
	else if (actualLevel == level1 /*&& actualLevel->completed*/) {
		if (hasWon) {
			actualLevel = level2;
			ActualLifes = 3;
			hasWon = false;
			isPlayerOnLevel = true;
		}
		else
		{
			whirlTimer = 200;
			hasLost = false;
			ActualLifes = 3;
			actualLevel->fruitsGeneratedInLevel = 0;
			isPlayerOnLevel = true;
		}
	}
	else if (actualLevel == level2 /*&& actualLevel->completed*/) {
		if (hasWon) {
			actualLevel = level3;
			ActualLifes = 3;
			hasWon = false;
			isPlayerOnLevel = true;
		}
		else
		{
			whirlTimer = 200;
			hasLost = false;
			ActualLifes = 3;
			actualLevel->fruitsGeneratedInLevel = 0;
			isPlayerOnLevel = true;
		}
	}
	else if (actualLevel == level3 /*&& actualLevel->completed*/) {
		if (hasWon) {
			actualLevel = level1;
			ActualLifes = 3;
			hasWon = false;
			isPlayerOnLevel = true;
		}
		else
		{
			whirlTimer = 200;
			hasLost = false;
			ActualLifes = 3;
			actualLevel->fruitsGeneratedInLevel = 0;
			isPlayerOnLevel = true;
		}
	}
}
void GameManager::callStarterSpring() {
	if (isPlayerOnLevel) return;
	//spring
	p3 = new Particle(MUELLE, { 0,30,-50 }, { 0,0,0 });
	pFR->addRegistry(f3, p3);
	_particles.push_back(p3);

	//call
	startNextLevel();
}
void GameManager::victoryFireworks() {
	for (size_t i = 0; i < 15; i++)
	{
		shootFirework(0);
	}
}
void GameManager::shootWeapon()
{
	if (!weapons.empty()) return;
	auto weap = physics->createRigidDynamic(physx::PxTransform(GetCamera()->getEye()));
	weap->setLinearVelocity({ GetCamera()->getDir() * 300 });
	weap->setAngularVelocity({ 0,0,0 });


	auto shape = CreateShape(physx::PxSphereGeometry(3.0)); weap->attachShape(*shape);
	physx::PxRigidBodyExt::setMassAndUpdateInertia(*weap, 2.5);

	auto x = new RenderItem(shape, weap, { 0.8, 0.8, 0.8, 1.0 });
	weap->setLinearDamping(0.99);
	weap->setName("Weapon");
	scene->addActor(*weap);



	DynamicBody* s = new DynamicBody();
	s->body = weap;/* s->_remaining_time = 5*/; s->rendIt = x; s->_alive = true; s->lifeTime = 1;
	//_bullets.push_back(s);
	weapons.push_back(s);

}

void GameManager::addFruit(double t)
{
	Vector4 col;
	DynamicBody* s = new DynamicBody();
	auto random = rand() % 6;
	if (random == 0) {
		col = { 0,0,0,1.0 };
		s->isBomb = true;
	}

	else col = { 0.0, 1, 0.0, 1.0 };
	actualLevel->fruitsGeneratedInLevel++;
	lastShoot = t;
	auto r = rand() % ((50 - 20) + 1) + 20;
	auto r2 = rand() % ((-50 - -80) + 1) + -80;

	auto fruit = physics->createRigidDynamic(physx::PxTransform({ (float)r,60,(float)r2 }));
	fruit->setLinearVelocity({ 0,20,0 });
	fruit->setAngularVelocity({ 0,0,0 });


	auto shape = CreateShape(physx::PxSphereGeometry(3.0)); fruit->attachShape(*shape);
	physx::PxRigidBodyExt::setMassAndUpdateInertia(*fruit, 2.5);

	auto x = new RenderItem(shape, fruit, col);
	//fruit->setLinearDamping(0.99);
	fruit->setName("Fruit");
	scene->addActor(*fruit);




	s->body = fruit;/* s->_remaining_time = 5*/; s->rendIt = x; s->_alive = true;
	//_bullets.push_back(s);
	fruits.push_back(s);
}
//Deletes the selected dynamic body 
void GameManager::deleteDynamicBody(DynamicBody* d, std::string s)
{
	//if (d->_alive) {
	if (s == "f") {
		d->body->setGlobalPose({ 1000000,0,0 });

	}

	else d->body->setGlobalPose({ 2000000,0,0 });
	d->_alive = false;

	d->body->release();
	DeregisterRenderItem(d->rendIt);
	//s->rI->color
	delete d;
	//}
}

void GameManager::renderLifes()
{
	display_text = "Lifes: " + std::to_string(ActualLifes);
	drawText(display_text, 100, 500);
}
void GameManager::renderPoints() {
	points_text = "Points: " + std::to_string(points);
	drawText(points_text, 600, 500);
}
void GameManager::stopLoseFeedback() {
	getParticleGenerator("Gaussian")->changeOperative();
	whirl->changeWhirl();
	glClearColor(0.3f, 0.4f, 0.5f, 1.0);
	p3->kill();
	/*p3 = new Particle(MUELLE, { 0,30,-50 }, { 0,0,0 });*/
	/*pFR->deleteParticleRegistry(p3);
	p3->setVelocity({ 0,0,0 });
	p3->clearForce();
	p3->setPosition({ 0, 30, -50 });*/

}
//Checks collisions between fruits and the floor
void GameManager::collisionsBetweenFruitsAndFloor(double t) {

	auto itFruits = fruits.begin();

	while (itFruits != fruits.end()) {

		if (physx::PxGeometryQuery::overlap((*itFruits)->rendIt->shape->getGeometry().sphere(), (*itFruits)->body->getGlobalPose(), floorItem->shape->getGeometry().box(), floor->getGlobalPose())) {

			ActualLifes--;

			deleteDynamicBody(*itFruits, "f");
			itFruits = fruits.erase(itFruits);

		}
		else {
			++itFruits;
		}
	}

}

//Checks collisions between fruits and weapons
void GameManager::collisionsBetweenFruitsAndWeapons(double t) {
	auto itFruits = fruits.begin();
	auto it2 = weapons.begin();

	while (it2 != weapons.end()) {
		while (itFruits != fruits.end()) {
			if (physx::PxGeometryQuery::overlap((*it2)->rendIt->shape->getGeometry().sphere(), (*it2)->body->getGlobalPose(), (*itFruits)->rendIt->shape->getGeometry().sphere(), (*itFruits)->body->getGlobalPose())) {

				if ((*itFruits)->isBomb) {
					ActualLifes = 0;
					deleteDynamicBody(*itFruits, "f");
					itFruits = fruits.erase(itFruits);
				}
				else
				{
					points++;
					deleteDynamicBody(*itFruits, "f");
					itFruits = fruits.erase(itFruits);
				}

			}
			else itFruits++;
		}
		(*it2)->lifeTime -= t;
		if ((*it2)->lifeTime <= 0) {

			deleteDynamicBody(*it2, "w");
			it2 = weapons.erase(it2);
		}

		else it2++;
	}
}

void GameManager::deleteAllFruits()
{
	auto itFruits = fruits.begin();

	while (itFruits != fruits.end()) {

		deleteDynamicBody(*itFruits, "f");
		itFruits = fruits.erase(itFruits);

	}
}
