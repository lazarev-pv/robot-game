#include <LevelObjects/Water.h>
#include <ChibiEngine/Resource/FileSystem.h>
#include <ChibiEngine/XMLParser/WorldParser.h>
#include <ChibiEngine/Physics/Physics.h>

using namespace std;
using namespace game;
using namespace glm;
using namespace rapidxml;


b2Vec2 ComputeCentroid(std::vector<b2Vec2> vs, float& area)
{
    int count = (int)vs.size();
    assert(count >= 3);

    b2Vec2 c; c.Set(0.0f, 0.0f);
    area = 0.0f;

    // pRef is the reference point for forming triangles.
    // It's location doesn't change the result (except for rounding error).
    b2Vec2 pRef(0.0f, 0.0f);

    const float32 inv3 = 1.0f / 3.0f;

    for (int32 i = 0; i < count; ++i)
    {
        // Triangle vertices.
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = vs[i];
        b2Vec2 p3 = i + 1 < count ? vs[i+1] : vs[0];

        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;

        float32 D = b2Cross(e1, e2);

        float32 triangleArea = 0.5f * D;
        area += triangleArea;

        // Area weighted centroid
        c += triangleArea * inv3 * (p1 + p2 + p3);
    }

    // Centroid
    if (area > b2_epsilon)
        c *= 1.0f / area;
    else
        area = 0;
    return c;
}

bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p) {
    return (cp2.x-cp1.x)*(p.y-cp1.y) > (cp2.y-cp1.y)*(p.x-cp1.x);
}

b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e) {
    b2Vec2 dc( cp1.x - cp2.x, cp1.y - cp2.y );
    b2Vec2 dp( s.x - e.x, s.y - e.y );
    float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
    float n2 = s.x * e.y - s.y * e.x;
    float n3 = 1.0 / (dc.x * dp.y - dc.y * dp.x);
    return b2Vec2( (n1*dp.x - n2*dc.x) * n3, (n1*dp.y - n2*dc.y) * n3);
}

//http://rosettacode.org/wiki/Sutherland-Hodgman_polygon_clipping#JavaScript
bool findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
{
    //currently this only handles polygon vs polygon
    if ( fA->GetShape()->GetType() != b2Shape::e_polygon ||
         fB->GetShape()->GetType() != b2Shape::e_polygon )
        return false;

    b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
    b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();

    //fill 'subject polygon' from fixtureA polygon
    for (int i = 0; i < polyA->GetVertexCount(); i++)
        outputVertices.push_back( fA->GetBody()->GetWorldPoint( polyA->GetVertex(i) ) );

    //fill 'clip polygon' from fixtureB polygon
    std::vector<b2Vec2> clipPolygon;
    for (int i = 0; i < polyB->GetVertexCount(); i++)
        clipPolygon.push_back( fB->GetBody()->GetWorldPoint( polyB->GetVertex(i) ) );

    b2Vec2 cp1 = clipPolygon[clipPolygon.size()-1];
    for (size_t j = 0; j < clipPolygon.size(); j++) {
        b2Vec2 cp2 = clipPolygon[j];
        if ( outputVertices.empty() )
            return false;
        std::vector<b2Vec2> inputList = outputVertices;
        outputVertices.clear();
        b2Vec2 s = inputList[inputList.size() - 1]; //last on the input list
        for (size_t i = 0; i < inputList.size(); i++) {
            b2Vec2 e = inputList[i];
            if (inside(cp1, cp2, e)) {
                if (!inside(cp1, cp2, s)) {
                    outputVertices.push_back( intersection(cp1, cp2, s, e) );
                }
                outputVertices.push_back(e);
            }
            else if (inside(cp1, cp2, s)) {
                outputVertices.push_back( intersection(cp1, cp2, s, e) );
            }
            s = e;
        }
        cp1 = cp2;
    }

    return !outputVertices.empty();
}

Water::Water(int uid, GeometrySlot* type,Layer* layer)
    :PhysLevelObject(uid,type, layer){
    procId = Game::getPhysicsSystem()->addAdditionalProcessing(bind(&Water::updatePhysics,this));
}

Water::~Water(){
    Game::getPhysicsSystem()->deleteAdditionalProcessing(procId);
}

void Water::BeginContact(b2Fixture* contactedFixture, b2Fixture* other){
	if(other->GetBody()->GetType()==b2_dynamicBody)
		contactedFixtures.insert(other);
}

void Water::EndContact(b2Fixture* contactedFixture, b2Fixture* other){
	if(other->GetBody()->GetType()==b2_dynamicBody)
		contactedFixtures.erase(other);
}

void Water::updatePhysics(){

	for(auto iter: contactedFixtures){
		b2Fixture* waterFixture = getBody()->GetFixtureList();
		b2Fixture* contactedFixture = iter;

		float density = waterFixture->GetDensity();

		std::vector<b2Vec2> intersectionPoints;
		if ( findIntersectionOfFixtures(waterFixture, contactedFixture, intersectionPoints) ) {

			//find centroid
			float area = 0;
			b2Vec2 centroid = ComputeCentroid( intersectionPoints, area);

			//apply buoyancy force
			float displacedMass = waterFixture->GetDensity() * area;
			b2Vec2 gravity( 0, -10 );
			b2Vec2 buoyancyForce = displacedMass * -gravity;
			contactedFixture->GetBody()->ApplyForce( buoyancyForce, centroid, true);

			//apply complex drag
			float dragMod = 0.25f;//adjust as desired
			float liftMod = 0.25f;//adjust as desired
			float maxDrag = 2000;//adjust as desired
			float maxLift = 500;//adjust as desired
			for (size_t i = 0; i < intersectionPoints.size(); i++) {
				b2Vec2 v0 = intersectionPoints[i];
				b2Vec2 v1 = intersectionPoints[(i+1)%intersectionPoints.size()];
				b2Vec2 midPoint = 0.5f * (v0+v1);

				//find relative velocity between object and fluid at edge midpoint
				b2Vec2 velDir = contactedFixture->GetBody()->GetLinearVelocityFromWorldPoint( midPoint ) -
						waterFixture->GetBody()->GetLinearVelocityFromWorldPoint( midPoint );
				float vel = velDir.Normalize();

				b2Vec2 edge = v1 - v0;
				float edgeLength = edge.Normalize();
				b2Vec2 normal = b2Cross(-1,edge);
				float dragDot = b2Dot(normal, velDir);
				if ( dragDot < 0 )
					continue;//normal points backwards - this is not a leading edge

				//apply drag
				float dragMag = dragDot * dragMod * edgeLength * density * vel * vel;
				dragMag = b2Min( dragMag, maxDrag );
				b2Vec2 dragForce = dragMag * -velDir;
				contactedFixture->GetBody()->ApplyForce( dragForce, midPoint, true);

				//apply lift
				float liftDot = b2Dot(edge, velDir);
				float liftMag =  dragDot * liftDot * liftMod * edgeLength * density * vel * vel;
				liftMag = b2Min( liftMag, maxLift );
				b2Vec2 liftDir = b2Cross(1,velDir);
				b2Vec2 liftForce = liftMag * liftDir;
				contactedFixture->GetBody()->ApplyForce( liftForce, midPoint, true );
			}
		}
	}
}




