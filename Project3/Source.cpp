//assignment 3
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

//window size
int winSizeX = 1400;
int winSizeY = 750;

//left button of mouse click status
int moving;
//left button of mouse click coordinate
int startx, starty;


//angle for rotate of whole body
static int angle_whole_body_scale = 1;
static int angle_whole_body_x = 0;
static int angle_whole_body_y = 0;
static int angle_whole_body_z = 0;

//torso rotation (angle)
static int torso_rotate_x = 0, torso_rotate_y=0, torso_rotate_z=0;

//neck parent, head child (angle)
/*Key: move neck: n, move head: h*/
static int neck = 0, head = 0;

//For shoulder, hand, foot ... ->>>/*Key: toggle left: l, toggle right: r*/
static int left_side_toggle = 0, right_side_toggle = 0;

//angle of body motion scale
static int part_motion_angle_scale = 5;
//shoulder grandparent, elbow parent, hand child (angle)
/*Key: move shoulder: q, move elbow: w, move hand: e*/
static int left_shoulder = 0, left_elbow = 0, left_hand = 0;
static int right_shoulder = 0, right_elbow = 0, right_hand = 0;

//upper leg grandparent, lower leg parent, foot child (angle)
/*Key: rotate upper_leg: a, rotate lower_leg: s, rotate foot: d*/
static int left_upper_leg = 0, left_lower_leg = 0, left_foot = 0;
static int right_upper_leg = 0, right_lower_leg = 0, right_foot = 0;

//surround object (angle)
static int surround_object_angle = 0;
static int surround_object_child_angle = 0;

//ear
static int ears_angle = 0;

//surround object translate
static GLfloat surround_object_child_translate = 0;

/*move far or near*/
GLfloat camera_distance = -10;

//camera center vector
GLfloat camera_center_x = 0;
GLfloat camera_center_y = 0;
GLfloat camera_center_scale = 0.1;

//lighting
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

//texture
GLuint texture; //the array for our texture

//state for animation
static int surround_object_child_translate_state = 0;
static int anim_head_state = 0;
static int anim_neck_state = 0;
static int anim_hand_state = 0;
static int anim_walk_state = 0;
static int anim_raise_sword_state = 0;
static int anim_bowing_state = 0;
static int anim_look_around_state = 0;
static int anim_bend_waist_state = 0;
static int anim_bend_waist_backward_state = 0;

//toggle for animation
static int anim_head_toggle = 0;
static int anim_neck_toggle = 0;
static int anim_hand_toggle = 0;
static int anim_walk_toggle = 0;
static int anim_raise_sword_toggle = 0;
static int anim_bowing_toggle = 0;
static int anim_look_around_toggle = 0;
static int anim_bend_waist_toggle = 0;
static int anim_bend_waist_backward_toggle = 0;
static int anim_dance_toggle = 0;

//toggle axis rotation variable
static int rotate_x_toggle = 0;
static int rotate_y_toggle = 0;
static int rotate_z_toggle = 0;

//toggle full screen
static int full_screen_toggle = 0;

/*---------------------------------------------------------------
 User Manual
	Key:

	//control view angle of whole model?
	Middle Button Down: Control view angle of entire model

	Move camera far/near? (Enlarge/Diminish View)
	Left Button Down(mouse motion to right): Move camera near
	Left Button Down(mouse motion to left): Move camera far

	Menu: Right Button (animation, auto axis rotation, ...)

	Rotate aixs?
	x: Rotate model in x-axis
	y: Rotate model in y-axis
	z: Rotate model in z-axis

	//which side to rotate?
	l: Toggle left side
	r: Toggle right side

	//which part of body to rotate?
	q: Move shoulder forward		 Q: Move shoulder backward
	w: Move elbow forward			 W: Move elbow backward
	e: Move hand forward			 E: Move hand backward

	a: Move upper leg forward		 A: Move upper leg backward
	s: Move lower leg forward		 S: Move lower leg backward
	d: Move foot forward			 D: Move foot backward

	c/C: Move torso x			
	v/V: Move torso y				
	b/B: Move torso z

	left-arrow key: camera center y move to left 
	right-arrow key: camera center y move to right

	up-arrow key: camera center x move to left
	down-arrow key: camera center x move to right
 -----------------------------------------------------------------*/

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our BMP file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture
	return texture; //return whether it was successfull
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void drawSurroundingObject()
{
	texture = LoadTexture("stonepath.bmp", 256, 256);
	glPushMatrix();
	{
		//disable effect of color material on textured floor
		glDisable(GL_COLOR_MATERIAL);
		glTranslatef(0, -3, 0);
		glScalef(8, 0.2, 8);
		
		//light source 3
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT2);
		GLfloat light[] = { 0.2 ,0.3, 1.0 };
		glLightfv(GL_LIGHT2, GL_EMISSION, light);
		glPushMatrix();
		{
			//update color material
			GLfloat ColorMaterialEmission2[] = { 0.0, 0.0, 1.0 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ColorMaterialEmission2);

			glRotatef(surround_object_angle, 0.0, 1.0, 0.0);
			glPushMatrix();
			{
				
				glTranslatef(0.2, 6, 0.2);
				glScalef(0.025, 1, 0.025);
				glTranslatef(0, surround_object_child_translate*2.5, 0);
				//draw ball around body
				glutSolidSphere(1, 30, 30);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0.1, 4, 0.1);
				glScalef(0.075, 3, 0.075);

				//for animation control
				glTranslatef(surround_object_child_translate, 0, surround_object_child_translate);
				glRotatef(surround_object_child_angle, 1.0, 0.0, 0.0);
				
				glRotatef(45, 1, 0, 0);
				glRotatef(45, 0, 1, 0);
				glRotatef(45, 0, 0, 1);
				glutSolidCube(1);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(-0.1, 4, 0.1);
				glScalef(0.075, 3, 0.075);

				//for animation control
				glTranslatef(-surround_object_child_translate, 0, surround_object_child_translate);
				glRotatef(surround_object_child_angle, 1.0, 0.0, 0.0);
				
				glRotatef(45, 1, 0, 0);
				glRotatef(45, 0, 1, 0);
				glRotatef(45, 0, 0, 1);
				glutSolidCube(1);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.1, 4, -0.1);
				glScalef(0.075, 3, 0.075);

				//for animation control
				glTranslatef(surround_object_child_translate, 0, -surround_object_child_translate);
				glRotatef(surround_object_child_angle, 0.0, 0.0, 1.0);
				
				glRotatef(45, 1, 0, 0);
				glRotatef(45, 0, 1, 0);
				glRotatef(45, 0, 0, 1);
				glutSolidCube(1);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(-0.1, 4, -0.1);
				glScalef(0.075, 3, 0.075);

				//for animation control
				glTranslatef(-surround_object_child_translate, 0, -surround_object_child_translate);
				glRotatef(surround_object_child_angle, 0.0, 0.0, 1.0);

				glRotatef(45, 1, 0, 0);
				glRotatef(45, 0, 1, 0);
				glRotatef(45, 0, 0, 1);
				glutSolidCube(1);
			}
			glPopMatrix();
	
			glTranslatef(0, -3, 0);
			glScalef(0.5, 1, 0.5);
			//draw upper floor
			glColor3f(0.1, 5, 0.1);
			glutSolidCube(1);
			glDisable(GL_LIGHT2);
		}
		glPopMatrix();

		//restore back the color material
		GLfloat ColorMaterialEmission[] = { 0.0, 0.0, 0.1 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ColorMaterialEmission);
	
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate
		glEnable(GL_TEXTURE_GEN_T);
		glCullFace(GL_FRONT_AND_BACK); //face culling 


		//draw upper floor
		glutSolidCube(1);
		
		glDisable(GL_TEXTURE_2D); //enable 2D texturing
		glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate
		glDisable(GL_TEXTURE_GEN_T);

		//enable back color material
		glEnable(GL_COLOR_MATERIAL);
	}
	glPopMatrix();
}

void display(void)
{
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//color material
	GLfloat ColorMaterialEmission[] = { 0.0, 0.0, 0.1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ColorMaterialEmission);

	glPushMatrix(); //start hierarchy
	{
		gluLookAt(0, 0, camera_distance, camera_center_x, camera_center_y, 0, 0, 1, 0);
		//set model to face front
		glRotatef(180, 0.0, 1.0, 0.0);

		glRotatef(angle_whole_body_x, 1.0, 0.0, 0.0);
		glRotatef(angle_whole_body_y, 0.0, 1.0, 0.0);
		glRotatef(angle_whole_body_z, 0.0, 0.0, 1.0);

		drawSurroundingObject();

		//start torso, torso is the center of the body
		glPushMatrix();
		{
			glRotatef((GLfloat)torso_rotate_x, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)torso_rotate_y, 0.0, 1.0, 0.0);
			glRotatef((GLfloat)torso_rotate_z, 0.0, 0.0, 1.0);

			//start chest
				glPushMatrix();
				{
					glTranslatef(0, 0.5, 0);
					glScalef(2, 1, 1.5);

					//draw chest
					glColor3f(0.6, 0.8, 0.4);
					glutSolidCube(1);
				}
				glPopMatrix();  //end chest


		
			//start neck
	
			glPushMatrix();
			{
				glTranslatef(0, 1.15, 0);
				glRotatef((GLfloat)neck, 0, 0, 1);
				glScalef(0.6, 0.3, 0.6);
				//start head
				glPushMatrix();
				{
					glTranslatef(0, 1.7, 0.0);
					glScalef(1, 1.5, 1);
					glRotatef((GLfloat)head, 0, 1, 0);

					glEnable(GL_LIGHT3);
					GLfloat eye_emission_light[] = { 1.0,1.0,1.0 };
					glLightfv(GL_LIGHT3, GL_EMISSION, eye_emission_light);
					GLfloat ColorMaterialEmission2[] = { 0.0, 1.0, 0.0 };
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ColorMaterialEmission2);
					//start left eye
					glPushMatrix();
					{
						glTranslatef(-0.4, 0.1, 0.9);
						glScalef(0.25, 0.2, 0.2);
						glutSolidSphere(1, 30, 30);
					}
					glPopMatrix(); //end left eye
				
					//start right eye
					glPushMatrix();
					{
						glTranslatef(0.4, 0.1, 0.9);
						glScalef(0.25, 0.2, 0.2);
						glutSolidSphere(1, 30, 30);
					}
					glPopMatrix(); //end right eye

					glDisable(GL_LIGHT3);
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ColorMaterialEmission);

					//start left ear
					glPushMatrix();
					{
						glTranslatef(-1.1, 0.1, 0.2);
						glScalef(0.3, 0.5, 0.5);
						glRotatef(ears_angle, 1, 0, 0);
						glColor3f(0.7, 0.8, 0.6);
						glutSolidCube(1);
					}
					glPopMatrix(); //end left ear

					//start right ear
					glPushMatrix();
					{
						glTranslatef(1.1, 0.1, 0.2);
						glScalef(0.3, 0.5, 0.5);
						glRotatef(ears_angle, 1, 0, 0);
						glColor3f(0.7, 0.8, 0.6);
						glutSolidCube(1);
					}
					glPopMatrix(); //end right ear

					//start mouth
					glPushMatrix();
					{
						glTranslatef(0, -0.5, 0.8);
						glScalef(0.7, 0.3, 0.5);
						glColor3f(0.7, 1.0, 0.1);
						glutSolidCube(1);
					}
					glPopMatrix();
					//draw head
					glColor3f(0.6, 0.8, 0.4);
					glutSolidSphere(1, 30, 30);
				}
				glPopMatrix(); //end head
			
				//draw neck
				glColor3f(0.6, 0.9, 0.3);
				glutSolidCube(1);


			}
			glPopMatrix(); //end neck




			//start left shoulder
			glPushMatrix();
			{
				glRotatef((GLfloat)left_shoulder, 1, 0, 0);
				glTranslatef(-1.2, 0.4, 0);
				glScalef(0.4, 1.2, 0.4);
				//start left elbow, inherit translation 
				glPushMatrix();
				{
					glRotatef((GLfloat)left_elbow, 0, 1, 0);
					glTranslatef(0, -0.9, 0);
					glScalef(1, 0.8, 1);

					//start left hand, inherit parent (left elbow)'s transformation
					glPushMatrix();
					{
						glRotatef((GLfloat)left_hand, 0, 1, 0);
						glTranslatef(0, -0.75, 0);
						glScalef(1, 0.5 , 1);
						//start sword 
						glPushMatrix();
						{
							//start sword's grip tail
							glPushMatrix();
							{
								glTranslatef(0, 0, -1.6);
								glScalef(0.4, 0.4, 1);
								glColor3f(0.6, 0.5, 0.2);
								glutSolidCube(1);

								//start sword's grip tail's diamond
								glPushMatrix();
								{
									glTranslatef(0, 0, -0.8);
									glScalef(1.2, 1.2, 0.4);
									glColor3f(0.6, 0.6, 0.1);
									glutSolidSphere(1, 30, 30);
								}
								glPopMatrix(); //end sword's grip tail's diamond
							}
							glPopMatrix(); //end sword's grip tail
						


							//start sword's grip
							glPushMatrix();
							{
								glTranslatef(0, 0, -0.8);
								glScalef(0.7, 0.7, 0.7);
								glColor3f(0.6, 0.3, 0.1);
								glutSolidCube(1);
							}
							glPopMatrix(); //end sword's grip

							//start sword's guard
							glPushMatrix();
							{
								glTranslatef(0, 0, 0.9);

								glScalef(2, 0.7, 0.7);

								glColor3f(0.6, 0.3, 0.1);
								glutSolidCube(1);
							}
							glPopMatrix(); //end sword's guard

							//start sword's body
							glPushMatrix();
							{
								glTranslatef(0, 0, 6);
								glScalef(1, 0.3, 10);
								glDisable(GL_LIGHTING);
								glLightfv(GL_LIGHT0, GL_POSITION, light_position);
								//draw weapon
								glColor3f(0.9, 1.0, 1.0);
								glutSolidCube(1);
								//we want the light source to be at the weapon
								//we disable lighting on the weapon as it will be dull because its surface is weaken by light
								//enable lighting for the rest
								glEnable(GL_LIGHTING);
								glEnable(GL_LIGHT0);
							}
							glPopMatrix(); //end sword's body

						}
						glPopMatrix(); //end sword

						//draw left hand
						glColor3f(0.6, 0.8, 0.2);
						glutSolidCube(1);
					}
					glPopMatrix();  //end left hand
				
					//draw left elbow
					glColor3f(0.6, 0.8, 0.4);
					glutSolidCube(1);
				}
				glPopMatrix();  //end left elbow
				//draw left shoulder
				glColor3f(0.6, 0.9, 0.3);
				glutSolidCube(1);
			}
			glPopMatrix();// end left shoulder

			//start right shoulder
			glPushMatrix();
			{
				glRotatef((GLfloat)right_shoulder, 1, 0, 0);
				glTranslatef(1.2, 0.4, 0);
				glScalef(0.4, 1.2, 0.4);

				//start right elbow, inherit translation 
				glPushMatrix();
				{
					glRotatef((GLfloat)right_elbow, 0, 1, 0);
					glTranslatef(0, -0.9, 0);
					glScalef(1, 0.8, 1);
					//start right hand, inherit parent (right elbow)'s transformation
					glPushMatrix();
					{
						glRotatef((GLfloat)right_hand, 0, 1, 0);
						glTranslatef(0, -0.75, 0);
						glScalef(1, 0.5, 1);
					
						//start shield
						glPushMatrix();
						{
							//start shield grip
							glPushMatrix();
							{
								glTranslatef(0.5, 0, 0);
								glScalef(0.5, 0.5, 0.5);
								glColor3f(1, 1, 1);
								glutSolidCube(1);
							}
							glPopMatrix(); //end shield grip

							//start shield inner body
							glPushMatrix();
							{
								glTranslatef(0.8, 0, 0);
								glRotatef(45, 1.0, 0.0, 0.0);
								glScalef(0.2, 4, 4);
								glColor3f(0.8, 0.6, 0.3);
								glutSolidCube(1);
							}
							glPopMatrix(); //end shield inner body

							//start shield outer body
							glPushMatrix();
							{
								glTranslatef(1.2, 0, 0);
								glRotatef(45, 1.0, 0.0, 0.0);
								glScalef(0.5, 3.0, 3.0);
								glColor3f(0.8, 0.7, 0.2);
								glutSolidCube(1);
							}
							glPopMatrix(); //end shield outer body

							//start shield outer body top diamond
							glPushMatrix();
							{
								//light_shader
								GLfloat light_diffuse[] = { 0.1,0.1,0.4 };
								glEnable(GL_LIGHT1);
								glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

								glTranslatef(1.5, 1.4, 0);
								glScalef(0.2, 0.2, 0.2);
								glColor3f(0.8, 1.0, 0.4);
								glutSolidSphere(1, 30, 30);
							}
							glPopMatrix(); //end shield outer body top diamond

							//start shield outer body middle diamond
							glPushMatrix();
							{
								glTranslatef(1.5, 0, 0);
								glScalef(0.5, 0.5, 0.5);
								glColor3f(0.8, 1.0, 1.0);
								glutSolidSphere(1, 30, 30);
								
							}
							glPopMatrix(); //end shield outer body middle diamond

						   //start shield outer body bottom diamond
							glPushMatrix();
							{
								glTranslatef(1.5, -1.4, 0);
								glScalef(0.2, 0.2, 0.2);
								glColor3f(0.8, 1.0, 0.4);
								glutSolidSphere(1, 30, 30);
							}
							glPopMatrix(); //end shield outer body top diamond

							//start shield outer body front diamond
							glPushMatrix();
							{
								glTranslatef(1.5, 0, 1.4);
								glScalef(0.2, 0.2, 0.2);
								glColor3f(0.8, 1.0, 0.4);
								glutSolidSphere(1, 30, 30);
							}
							glPopMatrix(); //end shield outer body front diamond
						
							//start shield outer body back diamond
							glPushMatrix();
							{
								glTranslatef(1.5, 0, -1.4);
								glScalef(0.2, 0.2, 0.2);
								glColor3f(0.8, 1.0, 0.4);
								glutSolidSphere(1, 30, 30);

								//disable the second light source
								glDisable(GL_LIGHT1);
							}
							glPopMatrix(); //end shield outer body back diamond

						}
						glPopMatrix(); //end shield

						//draw right hand
						glColor3f(0.6, 0.8, 0.2);
						glutSolidCube(1);
					}
					glPopMatrix(); //end right hand
				
					//draw right elbow
					glColor3f(0.6, 0.8, 0.4);
					glutSolidCube(1);
				}
				glPopMatrix();  //end right elbow

				//draw right shoulder
				glColor3f(0.6, 0.9, 0.3);
				glutSolidCube(1);
			}
			glPopMatrix(); // end right shoulder

			glTranslatef(0, -0.35, 0);
			glScalef(1.3, 0.7, 1.3);
			//draw torso
			glColor3f(0.6, 0.9, 0.3);
			glutSolidCube(1);
		}
		glPopMatrix(); //end torso

		//start upper left leg
		glPushMatrix();
		{
			glRotatef((GLfloat)left_upper_leg, 1, 0, 0);
			glTranslatef(-0.5, -1.1, 0);
			glScalef(0.5, 0.8, 0.5);

			//start lower left leg
			glPushMatrix();
			{
				glRotatef((GLfloat)left_lower_leg, 1, 0, 0);
				glTranslatef(0, -1, 0);

				//start left foot
				glPushMatrix();
				{
					glRotatef((GLfloat)left_foot, 1, 0, 0);
					glTranslatef(0, -0.75, 0.25);
					glScalef(1, 0.5, 1.5);
					//draw left foot
					glColor3f(0.6, 0.8, 0.4);
					glutSolidCube(1);
				}
				glPopMatrix(); //end left foot
				
				//draw lower left leg
				glColor3f(0.6, 0.8, 0.4);
				glutSolidCube(1);
			}
			glPopMatrix(); //end lower leg
			
			//draw upper left leg
			glColor3f(0.6, 0.9, 0.3);
			glutSolidCube(1);
		}
		glPopMatrix(); //end left upper leg

		//start upper right leg
		glPushMatrix();
		{
			glRotatef((GLfloat)right_upper_leg, 1, 0, 0);
			glTranslatef(0.5, -1.1, 0);
			glScalef(0.5, 0.8, 0.5);
			//start lower right leg
			glPushMatrix();
			{
				glRotatef((GLfloat)right_lower_leg, 1, 0, 0);
				glTranslatef(0, -1, 0);
				//start right foot
				glPushMatrix();
				{
					glRotatef((GLfloat)right_foot, 1, 0, 0);
					glTranslatef(0, -0.75, 0.25);
					glScalef(1, 0.5, 1.5);
					//draw right foot
					glColor3f(0.6, 0.8, 0.4);
					glutSolidCube(1);
				}
				glPopMatrix(); //end right foot

				//draw lower right leg
				glColor3f(0.6, 0.8, 0.4);
				glutSolidCube(1);
			}
			glPopMatrix(); //end lower leg

			//draw upper right leg
			glColor3f(0.6, 0.9, 0.3);
			glutSolidCube(1);
		}
		glPopMatrix(); //end upper right leg

	}
	glPopMatrix(); //end hierarchy

	//free the texture
	FreeTexture(texture);
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//distance from camera far 10px
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		angle_whole_body_x = (angle_whole_body_x + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;
	case 'X':
		angle_whole_body_x = (angle_whole_body_x - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		angle_whole_body_y = (angle_whole_body_y + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;
	case 'Y':
		angle_whole_body_y = (angle_whole_body_y - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;
	case 'z':
		angle_whole_body_z = (angle_whole_body_z + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;
	case 'Z':
		angle_whole_body_z = (angle_whole_body_z - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'c':
		torso_rotate_x = (torso_rotate_x + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'C':
		torso_rotate_x = (torso_rotate_x - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'v':
		torso_rotate_y = (torso_rotate_y + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'V':
		torso_rotate_y = (torso_rotate_y - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'b':
		torso_rotate_z = (torso_rotate_z + angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'B':
		torso_rotate_z = (torso_rotate_z - angle_whole_body_scale) % 360;
		glutPostRedisplay();
		break;

	case 'h':
		if (head < 20)
			head = (head + 5) % 25;
		glutPostRedisplay();
		break;
	case 'H':
		if (head > -20)
			head = (head - 5) % 25;
		break;
	case 'n':
		if (neck < 20)
			neck = (neck + 5) % 35;
		glutPostRedisplay();
		break;
	case 'N':
		if (neck > -20)
			neck = (neck - 5) % 35;
		glutPostRedisplay();
		break;
	case 'l':
		if (left_side_toggle == 0)
		{
			left_side_toggle = 1;
			std::cout << "Left side activated\n";
		}
		else
		{
			left_side_toggle = 0;
			std::cout << "Left side deactivated\n";
		}
		glutPostRedisplay();
		break;
	case 'r':
		if (right_side_toggle == 0)
		{
			right_side_toggle = 1;
			std::cout << "Right side activated\n";
		}
		else
		{
			right_side_toggle = 0;
			std::cout << "Right side deactivated\n";
		}
		glutPostRedisplay();
		break;
	case 'q':  // rotate shoulder backward
		if (left_side_toggle) 
			left_shoulder = (left_shoulder + part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_shoulder = (right_shoulder + part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;
	case 'Q':  //rotate shoulder forward
		if (left_side_toggle)
			left_shoulder = (left_shoulder - part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_shoulder = (right_shoulder - part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;

	case 'w':  //rotate elbow right
		if (left_side_toggle)
			left_elbow = (left_elbow + part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_elbow = (right_elbow + part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;
	case 'W':  //rotate elbow left
		if (left_side_toggle)
			left_elbow = (left_elbow - part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_elbow = (right_elbow - part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;
	case 'e':  //rotate hand right
		if (left_side_toggle)
			left_hand = (left_hand + part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_hand = (right_hand + part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;
	case 'E':  //rotate hand left
		if (left_side_toggle)
			left_hand = (left_hand - part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_hand = (right_hand - part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;

	case 'a': //rotate upper leg forward
		if (left_side_toggle)
			left_upper_leg = (left_upper_leg + part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_upper_leg = (right_upper_leg + part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;

	case 'A': //rotate upper leg backward
		if (left_side_toggle)
			left_upper_leg = (left_upper_leg - part_motion_angle_scale) % 360;
		if (right_side_toggle)
			right_upper_leg = (right_upper_leg - part_motion_angle_scale) % 360;
		glutPostRedisplay();
		break;

	case 's':  //rotate lower leg forward
		if (left_side_toggle)
			left_lower_leg = (left_lower_leg + part_motion_angle_scale) % 90;
		if (right_side_toggle)
			right_lower_leg = (right_lower_leg + part_motion_angle_scale) % 90;
		glutPostRedisplay();
		break;

	case 'S': //rotate lower leg backward
		if (left_side_toggle)
			left_lower_leg = (left_lower_leg - part_motion_angle_scale) % 90;
		if (right_side_toggle)
			right_lower_leg = (right_lower_leg - part_motion_angle_scale) % 90;
		glutPostRedisplay();
		break;

	case 'd': //rotate foot forward
		if (left_side_toggle)
			left_foot = (left_foot + part_motion_angle_scale) % 90;
		if (right_side_toggle)
			right_foot = (right_foot + part_motion_angle_scale) % 90;
		glutPostRedisplay();
		break;

	case 'D': //rotate foot backward
		if (left_side_toggle)
			left_foot = (left_foot - part_motion_angle_scale) % 90;
		if (right_side_toggle)
			right_foot = (right_foot - part_motion_angle_scale) % 90;
		glutPostRedisplay();
		break;
	
	default:
		break;
	}
}

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera_center_y -= camera_center_scale;
		break;
	case GLUT_KEY_DOWN:
		camera_center_y += camera_center_scale;
		break;
	case GLUT_KEY_LEFT:
		camera_center_x -= camera_center_scale;
		break;
	case GLUT_KEY_RIGHT:
		camera_center_x += camera_center_scale;
		break;
	}

	glutPostRedisplay();
}

//camera move
static int mouseX;  //record button down coordination
static int moveCamera;

/* mouse press detection */
static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {

			mouseX = x;
			moveCamera = 1;
		}
		if (state == GLUT_UP) {
			moveCamera = 0;
		}
	}
}
/* motion of mouse */
static void motion(int x, int y)
{
	if (moving) {
		angle_whole_body_y += (x - startx);
		angle_whole_body_x += (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}

	if (moveCamera) {
		GLfloat camera_distance_scale = 0.05;
		//motion to the left, diminish
		if (x < mouseX)
			camera_distance -= camera_distance_scale;
		//motion to the right, enlarge
		if (x >= mouseX)
			camera_distance += camera_distance_scale;

	}

}

/*Main Menu*/
void MainMenu(int id)
{
	//reset model
	if (id == 15) {
		//reset also axis rotation origin
		angle_whole_body_x = 0;
		angle_whole_body_y = 0;
		angle_whole_body_z = 0;

		torso_rotate_x = 0;
		torso_rotate_y = 0;
		torso_rotate_z = 0;

		neck = 0;
		head = 0;

		left_shoulder = 0;
		left_elbow = 0;
		left_hand = 0;

		right_shoulder = 0;
		right_elbow = 0;
		right_hand = 0;

		left_upper_leg = 0;
		left_lower_leg = 0;
		left_foot = 0;

		right_upper_leg = 0;
		right_lower_leg = 0;
		right_foot = 0;
	}
	if (id == 16)
	{
		if (full_screen_toggle == 0)
		{
			glutFullScreen();
			full_screen_toggle = 1;
		}
		else
		{
			glutPositionWindow(10, 30);
			glutReshapeWindow(winSizeX, winSizeY);
			full_screen_toggle = 0;
		}
	}

	if (id == 17)
		exit(0);
}
/* Animation Menu*/
void AnimationMenu(int id)
{
	if (id == 0)
	{
		anim_head_toggle = 0;
		anim_neck_toggle = 0;
		anim_hand_toggle = 0;
		anim_walk_toggle = 0;
		anim_raise_sword_toggle = 0;
		anim_bowing_toggle = 0;
		anim_look_around_toggle = 0;
		anim_bend_waist_toggle = 0;
		anim_bend_waist_backward_toggle = 0;
		anim_dance_toggle = 0;
	}
	if (id == 1)
	{
		if (anim_head_toggle == 0)
			anim_head_toggle = 1;
		else
			anim_head_toggle = 0;
	}

	if (id == 2)
	{
		if (anim_neck_toggle == 0)
			anim_neck_toggle = 1;
		else
			anim_neck_toggle = 0;
	}

	if (id == 3)
	{
		if (anim_hand_toggle == 0)
			anim_hand_toggle = 1;
		else
			anim_hand_toggle = 0;
	}

	if (id == 4)
	{
		if (anim_walk_toggle == 0)
			anim_walk_toggle = 1;
		else
			anim_walk_toggle = 0;
	}

	if (id == 5)
	{
		if (anim_raise_sword_toggle == 0)
			anim_raise_sword_toggle = 1;
		else
			anim_raise_sword_toggle = 0;
	}

	if (id == 6)
	{
		if (anim_bowing_toggle == 0)
			anim_bowing_toggle = 1;
		else
			anim_bowing_toggle = 0;
	}
	if (id == 7)
	{
		if (anim_look_around_toggle == 0)
			anim_look_around_toggle = 1;
		else
			anim_look_around_toggle = 0;
	}
	if (id == 8)
	{
		if (anim_bend_waist_toggle == 0)
			anim_bend_waist_toggle = 1;
		else
			anim_bend_waist_toggle = 0;
	}
	if (id == 9)
	{
		if (anim_bend_waist_backward_toggle == 0)
			anim_bend_waist_backward_toggle = 1;
		else
			anim_bend_waist_backward_toggle = 0;
	}
	if (id == 10)
	{
		//start dance
		if (anim_dance_toggle == 0)
		{
			anim_head_toggle = 1;
			anim_neck_toggle = 1;
			anim_hand_toggle = 1;
			anim_walk_toggle = 1;
			anim_bowing_toggle = 1;
			anim_look_around_toggle = 1;
			anim_bend_waist_toggle = 1;
		}
		else
		{
			anim_head_toggle = 0;
			anim_neck_toggle = 0;
			anim_hand_toggle = 0;
			anim_walk_toggle = 0;
			anim_bowing_toggle = 0;
			anim_look_around_toggle = 0;
			anim_bend_waist_toggle = 0;
		}
	}
}
/*Axis Rotation Menu*/
void AxisRotationMenu(int id)
{
	if (id == 11)
	{
		rotate_x_toggle = 0;
		rotate_y_toggle = 0;
		rotate_z_toggle = 0;
	}
	if (id == 12)
	{
		if (rotate_x_toggle == 0)
			rotate_x_toggle = 1;
		else
			rotate_x_toggle = 0;
	}

	if (id == 13)
	{
		if (rotate_y_toggle == 0)
			rotate_y_toggle = 1;
		else
			rotate_y_toggle = 0;
	}

	if (id == 14)
	{
		if (rotate_z_toggle == 0)
			rotate_z_toggle = 1;
		else
			rotate_z_toggle = 0;
	}

}
/*Item Insertion Menu*/
void Menu()
{
	int anim_submenu = glutCreateMenu(AnimationMenu);
	glutAddMenuEntry("Cancel All Animation", 0);
	glutAddMenuEntry("Toggle Head", 1);
	glutAddMenuEntry("Toggle Neck", 2);
	glutAddMenuEntry("Toggle Hand", 3);
	glutAddMenuEntry("Toggle Walk", 4);
	glutAddMenuEntry("Toggle Raise Sword", 5);
	glutAddMenuEntry("Toggle Bowing", 6);
	glutAddMenuEntry("Toggle Look Around", 7);
	glutAddMenuEntry("Toggle Bend Waist", 8);
	glutAddMenuEntry("Toggle Bend Waist Backward", 9);
	glutAddMenuEntry("Toggle Dance", 10);
	
	int axis_rotation_submenu = glutCreateMenu(AxisRotationMenu);
	glutAddMenuEntry("Cancel All Axis Rotation", 11);
	glutAddMenuEntry("Toggle x-axis Rotation", 12);
	glutAddMenuEntry("Toggle y-axis Rotation", 13);
	glutAddMenuEntry("Toggle z-axis Rotation", 14);

	glutCreateMenu(MainMenu);
	glutAddSubMenu("Animation", anim_submenu);
	glutAddSubMenu("Axis Rotation", axis_rotation_submenu);
	
	glutAddMenuEntry("Reset", 15);
	
	glutAddMenuEntry("Toggle Full Screen", 16);
	glutAddMenuEntry("Exit", 17);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



//for animation
static void timer(int)
{
	//if axis rotation activated
	if (rotate_x_toggle) angle_whole_body_x = (angle_whole_body_x + 1) % 360;
	if (rotate_y_toggle) angle_whole_body_y = (angle_whole_body_y + 1) % 360;
	if (rotate_z_toggle) angle_whole_body_z = (angle_whole_body_z + 1) % 360;

	//rotate surrounding object
	surround_object_angle = (surround_object_angle + 2) % 360;
	//rotate surrounding object child
	surround_object_child_angle = (surround_object_child_angle + 3) % 360;

	//rotate ears
	ears_angle = (ears_angle + 5) % 360;

	//translate surround object
	switch (surround_object_child_translate_state)
	{
		case 0:
			if (surround_object_child_translate < 5.0)
				surround_object_child_translate += 0.05;
			else surround_object_child_translate_state = 1;
			
			break;
		case 1:
			if (surround_object_child_translate > 0.0)
				surround_object_child_translate -= 0.05;
			else surround_object_child_translate_state = 0;
			break;
	}


	if (anim_head_toggle)
	{
		switch (anim_head_state)
		{
			case 0:
				if (head < 20)
					head++;
				else
					anim_head_state = 1;
				break;
			case 1:
				if (head > -20)
					head--;
				else
					anim_head_state = 0;
				break;
		}
	}

	if (anim_neck_toggle)
	{
		switch (anim_neck_state)
		{
			case 0:
				if (neck < 20)
					neck++;
				else
					anim_neck_state = 1;
				break;
			case 1:
				if (neck > -20)
					neck--;
				else
					anim_neck_state = 0;
				break;
		}
	}

	if (anim_hand_toggle)
	{
		switch (anim_hand_state)
		{
			case 0:
				if (left_shoulder < 45)
				{
					left_shoulder++;
					right_shoulder--;
					left_hand--;
					right_hand--;
				}
				else
					anim_hand_state = 1;
				break;

			case 1:
				if (left_shoulder > -45)
				{
					left_shoulder--;
					right_shoulder++;
					left_hand++;
					right_hand++;

				}
				else
					anim_hand_state = 0;
				break;
		}
	}

	if (anim_walk_toggle)
	{
		switch (anim_walk_state)
		{
			case 0:
				if (left_upper_leg > -45)
				{
					left_upper_leg--;
					right_upper_leg++;
					left_lower_leg++;
					right_lower_leg--;
					left_foot--;
					right_foot++;
				}
				else
					anim_walk_state = 1;
				break;
			case 1:
				if (left_upper_leg < 45)
				{
					left_upper_leg++;
					right_upper_leg--;
					left_lower_leg--;
					right_lower_leg++;
					left_foot++;
					right_foot--;
				}
				else
					anim_walk_state = 0;
				break;
		}
	}


	if (anim_raise_sword_toggle)
	{
		switch (anim_raise_sword_state)
		{
			case 0:
				if (left_shoulder > -80)
				{
					left_shoulder--;
					left_hand++;
				}
				else
					anim_raise_sword_state = 1;
				break;
			case 1:
				if (left_shoulder < 0)
				{
					left_shoulder++;
					left_hand--;
				}
				else
					anim_raise_sword_state = 0;
				break;
		}
	}
	
	if (anim_bowing_toggle)
	{
		
		switch (anim_bowing_state)
		{
			case 0:
				if (torso_rotate_x < 90)
					torso_rotate_x++;
				else anim_bowing_state = 1;
				break;
			case 1:
				if (torso_rotate_x > 0)
					torso_rotate_x--;
				else anim_bowing_state = 0;
				break;
		}
	}

	if (anim_look_around_toggle)
	{
		switch (anim_look_around_state)
		{
			case 0:
				if (torso_rotate_y < 45)
					torso_rotate_y++;
				else
					anim_look_around_state = 1;
				break;
			case 1:
				if (torso_rotate_y > -45)
					torso_rotate_y--;
				else
					anim_look_around_state = 0;
				break;
		}
	}

	if (anim_bend_waist_toggle)
	{
		switch (anim_bend_waist_state)
		{
			case 0:
				if (torso_rotate_z < 45)
					torso_rotate_z++;
				else
					anim_bend_waist_state = 1;
				break;
			case 1:
				if (torso_rotate_z > -45)
					torso_rotate_z--;
				else
					anim_bend_waist_state = 0;
				break;
		}
	}

	if (anim_bend_waist_backward_toggle)
	{
		switch (anim_bend_waist_backward_state)
		{
			case 0:
				if (torso_rotate_x > -45)
				{
					torso_rotate_x--;
					left_upper_leg++;
					right_upper_leg++;
					left_lower_leg++;
					right_lower_leg++;
					left_foot++;
					right_foot++;
					left_shoulder--;
				}
				else
					anim_bend_waist_backward_state = 1;
				break;
			case 1:
				if (torso_rotate_x < 0)
				{
					torso_rotate_x++;
					left_upper_leg--;
					right_upper_leg--;
					left_lower_leg--;
					right_lower_leg--;
					left_foot--;
					right_foot--;
					left_shoulder++;
				}
				else
					anim_bend_waist_backward_state = 0;
				break;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1000/60 , timer, 0);

}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
	glutInitWindowSize(winSizeX, winSizeY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(0, timer, 0);
	
	//create menu for animation
	Menu();
	glutMainLoop();
	return 0;
}